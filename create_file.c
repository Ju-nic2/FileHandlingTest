#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_DATA_SIZE 49
#define RECORDSIZE 250
struct record{
	char name[MAX_DATA_SIZE+1];
	char number[MAX_DATA_SIZE+1];
	char addr[MAX_DATA_SIZE+1];
	char major[MAX_DATA_SIZE+1];
	char submajor[MAX_DATA_SIZE+1];
};

void InitBuffer(struct record *rbuf);
void CreateFile(int fd,int recordnum);

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr,"please write <record num> <file name>");
		exit(1);
	}

	int fd;
	if((fd = open(argv[2],O_CREAT|O_WRONLY|O_TRUNC,0644)) < 0 )
	{
		fprintf(stderr,"file open error");
		exit(1);
	}
	CreateFile(fd,atoi(argv[1]));
}


void InitBuffer(struct record *rbuf)
{
	for(int i = 0; i < MAX_DATA_SIZE; i++)
	{
		rbuf->name[i] = 'n';
		rbuf->number[i] = 'N';
		rbuf->addr[i] = 'a';
		rbuf->major[i] = 'm';
		rbuf->submajor[i] = 's';
	}
	rbuf->name[MAX_DATA_SIZE] = '\0';
	rbuf->number[MAX_DATA_SIZE] = '\0';
	rbuf->addr[MAX_DATA_SIZE] = '\0';
	rbuf->major[MAX_DATA_SIZE] = '\0';
	rbuf->submajor[MAX_DATA_SIZE] = '\0';
}

void CreateFile(int fd,int n)
{
	//make buffer, and fill with charater
	struct record rbuf;
	InitBuffer(&rbuf);
	//write record header
	write(fd,&n,4);

	//move to write records, next to header
	lseek(fd,4,SEEK_SET);
	for(int i = 0; i<n; i++)
	{
		write(fd,&rbuf,sizeof(rbuf));
	}
}

