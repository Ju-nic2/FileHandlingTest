#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define bufsize 10
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char*argv[])
{
	char buf[bufsize];
	int orginfd;
	int copyfd;
	off_t fsize;
	//argument form check
	if(argc != 3)
	{
		fprintf(stderr, " Please write <OriginFile>  <CopyFile>");
		exit(1);
	}
	//file open error check
	if((orginfd = open(argv[1],O_RDONLY)) < 0 )
	{
		fprintf(stderr, "There is no file %s",argv[1]);
		exit(1);
	}
	//file open error check if same file name exist error
	if((copyfd = open(argv[2], O_CREAT|O_EXCL|O_WRONLY|O_APPEND ,CREATE_MODE)) < 0)
	{
		fprintf(stderr,"Create Error\n");
		exit(0);
	}
	
	//move file discripter to first of file
	lseek(orginfd,0,SEEK_SET);

	while(1)
	{
		//to know the number of strings, when read file 
		int length = read(orginfd,buf,10);
		write(copyfd,buf,length);
		//when end of file length will smaller than 10
		if(length < 10)
			break;
	}
	close(orginfd);
	close(copyfd);
}







