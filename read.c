#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define BUFSIZE 10

int main(int argc, char* argv[])
{
	//argument form chenck
	if(argc != 4)
	{
		fprintf(stderr,"Please write File name, Offset , Bytes\n");
		exit(1);
	}
	//read buffer
	char buf[BUFSIZE];

	off_t offset = atoi(argv[2]);
	off_t bytes = atoi(argv[3]);
	off_t filesize;
	//file discripter check
	int fd = open(argv[1],O_RDONLY);
	if(fd < 0)
	{
		fprintf(stderr,"There is no file\n");
		exit(1);
	}
	filesize = lseek(fd,0,SEEK_END);
	//offset check
	if(offset > filesize)
	{
		fprintf(stderr,"No data in offset : %ld, please set offset under %ld",offset,lseek(fd,0,SEEK_END));
		exit(1);
	}
	//read bytes check 
	if(bytes > filesize-offset)
	{
		bytes = filesize - offset;
	}

	//move to start read
	lseek(fd,offset,SEEK_SET);
	//buffer size check wite bytes size
	if(bytes <= BUFSIZE)
	{
		int length = read(fd,buf,bytes);
		write(1,buf,length);
	}else{
		//n time read with buffer 
		for(int i = 0; i < ((int)bytes/BUFSIZE); i++)
		{
			read(fd,buf,BUFSIZE);
			write(1,buf,BUFSIZE);
		}
		//read remained data
		off_t remainsize = bytes%BUFSIZE;
		read(fd,buf,remainsize);
		write(1,buf,remainsize);
	}
	
}
