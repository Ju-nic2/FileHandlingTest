#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blockmap.h"

struct Mapping{
	int table[BLOCKS_PER_DEVICE-1];
	int freeblock;
};
struct Mapping Mapping;

void ftl_setFreeBlock();
void dd_read(int ppn, char*pagebuf);
void dd_write(int ppn,char*pagebuf);
void dd_erase(int pbn);

void ftl_open()
{
	// address mapping table 초기화 또는 복구
	// free block's pbn 초기화
    // address mapping table에서 lbn 수는 DATABLKS_PER_DEVICE 동일
	int lbn;
	char *pagebuf;
	pagebuf = (char*)malloc(PAGE_SIZE);
	int list[BLOCKS_PER_DEVICE];
	for(int i = 0 ; i<BLOCKS_PER_DEVICE;i++)
	{
		Mapping.table[i] = -1;
	}
	for(int i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		dd_read(i*PAGES_PER_BLOCK,pagebuf);
		memcpy(&lbn,&pagebuf[SECTOR_SIZE],sizeof(int));
		if(lbn > -1)
			Mapping.table[lbn] = i;
	}
	free(pagebuf);
	
	ftl_setFreeBlock();
	return;
}

// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있음
void ftl_read(int lsn, char *sectorbuf)
{
	if(lsn >= ((BLOCKS_PER_DEVICE-1)*PAGES_PER_BLOCK))
	{
		fprintf(stderr,"lsn's Maximum is 60");
		return;
	}
	if(Mapping.table[lsn/PAGES_PER_BLOCK] == -1)
	{
		fprintf(stderr,"There is no data in %d block",lsn);
		return;
	}
	char pagebuf[PAGE_SIZE];
	int pbn = Mapping.table[lsn/PAGES_PER_BLOCK];
	int offset = lsn%PAGES_PER_BLOCK;
	int ppn = pbn*PAGES_PER_BLOCK+offset;
	int savedlsn;
	dd_read(ppn,pagebuf);
	
	memcpy(&savedlsn,&pagebuf[SECTOR_SIZE+4],4);
	if(savedlsn == -1)
	{
		fprintf(stderr,"There is no data in %d page",lsn);
		return;
	}
	else{
		memcpy(sectorbuf,pagebuf,SECTOR_SIZE);
		return;
	}
}
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있음
void ftl_write(int lsn, char *sectorbuf)
{
	if(lsn >= ((BLOCKS_PER_DEVICE-1)*PAGES_PER_BLOCK))
	{
		fprintf(stderr,"lsn's Maximum is 60\n");
		return;
	}

	char pagebuf[PAGE_SIZE];
	int lbn = lsn/PAGES_PER_BLOCK;
	int pbn = Mapping.table[lbn];
	int offset = lsn%PAGES_PER_BLOCK;	

	//first write in block
	if(pbn == -1)
	{
		for(int i = 0; i<BLOCKS_PER_DEVICE; i++)
		{
			int check = 0;
			char *tmppage = (char*)malloc(PAGE_SIZE);
			dd_read(i*PAGES_PER_BLOCK,tmppage);
			memcpy(&check,&tmppage[SECTOR_SIZE],4);
			if(check == -1){
				pbn = i;
				break;
			}
			free(tmppage);
		}
		int ppn = pbn*PAGES_PER_BLOCK + offset;
		memcpy(pagebuf,sectorbuf,SECTOR_SIZE);	
		if(offset == 0){
			//ppn is first page of block
			//write data lsn -> ppn
			memcpy(&pagebuf[SECTOR_SIZE],&lbn,4);
			memcpy(&pagebuf[SECTOR_SIZE+4],&lsn,4);
			dd_write(ppn,pagebuf);
		}else{
			//write data lsn -> ppn
			memcpy(&pagebuf[SECTOR_SIZE+4],&lsn,4);
			dd_write(ppn,pagebuf);
			//write lbn in First page of pbn
			ppn = pbn*PAGES_PER_BLOCK;
			dd_read(ppn,pagebuf);
			memcpy(&pagebuf[SECTOR_SIZE],&lbn,4);
			dd_write(ppn,pagebuf);
		}

		Mapping.table[lbn] = pbn;
		return;
	}else{
		int ppn = pbn*PAGES_PER_BLOCK + offset;
		int lsnInFile;
		dd_read(ppn,pagebuf);
		memcpy(&lsnInFile,&pagebuf[SECTOR_SIZE+4],4);
		
		//first write in page
		if(lsnInFile == -1)
		{
			memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
			memcpy(&pagebuf[SECTOR_SIZE+4],&lsn,4);
			dd_write(ppn,pagebuf);
			return;

		}
		//no in_place update
		else{
			int originppn = pbn*PAGES_PER_BLOCK + offset;
			int newpbn = Mapping.freeblock;
			int newppn = newpbn*PAGES_PER_BLOCK + offset;
			for(int i = 0; i<PAGES_PER_BLOCK; i++)
			{
				if(i != offset){
					dd_read(pbn*PAGES_PER_BLOCK+i,pagebuf);
					dd_write(newpbn*PAGES_PER_BLOCK+i,pagebuf);
				}
			}
			dd_read(originppn,pagebuf);
			memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
			dd_write(newppn,pagebuf);
			
			//update Table
			dd_erase(pbn);
			Mapping.table[lbn] = newpbn;
			ftl_setFreeBlock();
			return;
		}
				
		
	}


	return;
}

void ftl_print()
{
	printf("lbn pbn\n");
	for(int i = 0; i<BLOCKS_PER_DEVICE-1;i++)
	{
		printf("%d %d\n",i,Mapping.table[i]);
	}
	printf("free block's pbn=%d\n",Mapping.freeblock);

	return;
}

void ftl_setFreeBlock()
{
	int list[BLOCKS_PER_DEVICE]= {0,};

	for(int i =0; i<BLOCKS_PER_DEVICE-1;i++)
	{
		if(Mapping.table[i] != -1)
			list[Mapping.table[i]] = 1;
	}

	for(int i = 0; i<BLOCKS_PER_DEVICE; i++)
	{
		if(list[i] != 1)
			Mapping.freeblock = i;
	}
			
}

