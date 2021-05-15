#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
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
#define SUFFLE_NUM 1000

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);
void ReadFile_Ran(int *read_order_list,int fd,int num_of_records);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"please write <filename>");
		exit(1);
	}
	int fd;
	if((fd = open(argv[1],O_RDONLY)) < 0)
	{
		fprintf(stderr,"file open error");
		exit(1);
	}
	//read record header
	int num_of_records;
	read(fd,&num_of_records,4);
	int *read_order_list = malloc(sizeof(int)*num_of_records);
	
	GenRecordSequence(read_order_list, num_of_records);
	
	ReadFile_Ran(read_order_list,fd,num_of_records);
	return 0;
}

void ReadFile_Ran(int *list,int fd,int n)
{
	struct record buffer;
	int recordi;

	struct timeval start,end;

	gettimeofday(&start,NULL);
	for(int i = 0; i<n; i++)
	{
		recordi = list[i];
		lseek(fd,((recordi*RECORDSIZE)+4),SEEK_SET);
		read(fd,&buffer,sizeof(buffer));
	}
	gettimeofday(&end,NULL);

	printf("record : %d ", n);
	printf("elapsed_time: %dus\n",(int)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec)));
}
void GenRecordSequence(int *list, int n)
{
	int i, j, k;

	srand((unsigned int)time(0));

	for(i=0; i<n; i++)
	{
		list[i] = i;
	}
	
	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
