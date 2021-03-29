#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define Line_size 512

//define max File size = 1023*512 bytes;


int main(int argc, char*argv[])
{
	off_t offset;
	int fd;
	char buf[Line_size][BUFFER_SIZE];

	if(argc != 4)
	{
		fprintf(stderr,"Please Write filename, offset, data");
		exit(1);
	}
	offset = atoi(argv[2]);
	
	
	if((fd = open(argv[1],O_RDWR)) < 0)
	{
		fprintf(stderr, "There is no file");
		exit(1);
	}
	//move pointer to copy the record
	lseek(fd,(off_t)offset,SEEK_SET);
	
	//read and write
	{
		int length = 0;
		int count = 0;
		while((length = read(fd,buf[count],BUFFER_SIZE-1)) > 0)
		{
			//file have more 1023bytes
			if(length == 1023)
			{
				buf[count][1023] = '\0';
				count++;
			}else{
				buf[count][length] = '\0';
				count++;
			}

			if(count == 512)
			{
				fprintf(stderr,"It's too Big file size, So you can't insert data");
				exit(1);
			}
		}
		//move to insert data
		lseek(fd,offset,SEEK_SET);
		write(fd,argv[3],strlen(argv[3]));
		
		//move to write origin data
		lseek(fd,offset+strlen(argv[3]),SEEK_SET);
	
		for(int i = 0; i<=count; i++)
		{
			write(fd,buf[i],strlen(buf[i]));
		}

	}
	close(fd);
}
