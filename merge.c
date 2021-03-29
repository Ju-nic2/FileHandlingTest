#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
	//argument form check
	if(argc != 4)
	{
		fprintf(stderr, "Please Write File1 File2 File3");
		exit(1);
	}
	
	char buf;
	int fd,fd1,fd2;

	fd = open(argv[1],O_CREAT|O_WRONLY|O_APPEND,0666);
	fd1 = open(argv[2],O_RDONLY);
	fd2 = open(argv[3],O_RDONLY);
	//open error check
	if(fd < 0 || fd1 < 0 || fd2 < 0)
	{
		fprintf(stderr, "Please Write File1 File2 File3");
		exit(1);
	}
	//copy file2 into file1
	while(read(fd1,&buf,1) != 0)
	{
		write(fd,&buf,1);
	}
	close(fd1);
	//copy file3 into file1
	while(read(fd2,&buf,1) != 0)
	{
		write(fd,&buf,1);
	}
	close(fd2);
	close(fd);
}
