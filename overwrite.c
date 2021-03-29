#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
int main(int argc, char*argv[])
{
	//argument form check
	if(argc != 4)
	{
		fprintf(stderr, "Please Write Filename, Offset, Data\n");
		exit(1);
	}
	int fd;
	off_t offset = atoi(argv[2]);
	off_t filesize;
	//open error check
	if((fd = open(argv[1],O_RDWR)) < 0)
	{
		fprintf(stderr,"There is no file");
		exit(1);
	}
	//get file size
	filesize  = lseek(fd,(off_t)0,SEEK_END);
	
	//if offset is lager than filesize, write() can make hole in file
	if(offset > filesize)
	{
		fprintf(stderr,"Hole Making");
		printf("MAX offset is : %ld",filesize);
		exit(1);
	}else
		lseek(fd,offset,SEEK_SET);

	write(fd,argv[3],strlen(argv[3]));
	
	close(fd);
}

