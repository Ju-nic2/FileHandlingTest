#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#define MAX_DATA_SIZE 49
#define RECORDSIZE 250
struct record{
	char name[MAX_DATA_SIZE+1];
	char number[MAX_DATA_SIZE+1];
	char addr[MAX_DATA_SIZE+1];
	char major[MAX_DATA_SIZE+1];
	char submajor[MAX_DATA_SIZE+1];
};

void ReadFile_Seq(int fd);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Please write <filename>");
		exit(1);
	}

	int fd;
	if((fd = open(argv[1],O_RDONLY)) <  0)
	{
		fprintf(stderr,"File open Error");
		exit(1);
	}

	ReadFile_Seq(fd);
	return 0;
}

void ReadFile_Seq(int fd)
{
	int totalRecords;
	read(fd,&totalRecords,4);

	struct record buffer;
	//variables to check time
	struct timeval start, end;;

	gettimeofday(&start,NULL);
	for(int i=0; i<totalRecords; i++){
		read(fd,&buffer,sizeof(buffer));
	}
	gettimeofday(&end,NULL);
	printf("records: %d ",totalRecords);
	printf("elapsed_time: %d us\n",(int)((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec));
}
