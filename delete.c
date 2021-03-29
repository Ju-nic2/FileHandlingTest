#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define LINE_SIZE 512
//define max file size 1024*512 bytes

int main(int argc, char* argv[])
{
	int fd;
	off_t offset;
	off_t filesize;
	int bytes;
	char buf[LINE_SIZE][BUFFER_SIZE];
	//argument check
	if(argc != 4)
	{
		fprintf(stderr,"Please Write filename offset bytes");
		exit(1);
	}
	//get data
	offset = atoi(argv[2]);
	bytes = atoi(argv[3]);
	
	//oepn error chenck
	if((fd = open(argv[1],O_RDONLY)) < 0 )
	{
		fprintf(stderr, "File Open Error");
		exit(1);
	}
	
	//if offset is bigger than fileseize , there is no data
	filesize = lseek(fd,0,SEEK_END);
	if(offset > filesize)
	{
		fprintf(stderr,"There are no data in offset %ld",offset);
		exit(1);
	}
	int count = 0;
	//offset is smaller than buffer_size
	if(offset < BUFFER_SIZE)
	{
		int length = 0;
		lseek(fd,0,SEEK_SET);
		//read data 0 ~ offset
		length = read(fd,buf[count],offset);
		buf[count][length] = '\0';
		count++;
		
		//read data exept offset ~ offset+bytes
		lseek(fd,(off_t)bytes,SEEK_CUR);

		//read data offset+bytes ~ end of file
		while((length = read(fd,buf[count],BUFFER_SIZE-1)) > 0)
		{
			printf("length : %d",length);
			//file have more 1024bytes
			if(length == 1023)
			{
				buf[count][1023] = '\0';
				count++;
			}else{
				buf[count][length] = '\0';
				count++;
			}
			printf("count : %d\n",count);
		}
	}
	//offset is bigger than buffer_size
	else{
		int length = 0;
		lseek(fd,0,SEEK_SET);

		for(int i = 0; i<(offset/BUFFER_SIZE); i++)
		{
			read(fd,buf[count++],BUFFER_SIZE-1);
			buf[count++][1023] = '\0';
		}

		length = read(fd, buf[count],offset-BUFFER_SIZE);
		buf[count++][length] = '\0';
		lseek(fd,(off_t)bytes,SEEK_CUR);

		while((length = read(fd,buf[count],BUFFER_SIZE-1)) > 0)
		{
			printf("length : %d",length);
			//file have more 1024bytes
			if(length == 1023)
			{
				buf[count][1023] = '\0';
				count++;
			}else{
				buf[count][length] = '\0';
				count++;
			}
			printf("count : %d\n",count);
		}
	}

	close(fd);
	//delete all data in file to make file size = 0 
	if((fd = open(argv[1],O_WRONLY|O_TRUNC)) < 0)
	{
		fprintf(stderr,"FIle open Error 2");
		exit(1);
	}
	//write updated data
	printf("final count : %d\n",count);
	for(int i = 0; i<count; i++)
	{
		printf("buf size : %ld,strlen(buf[i]) : %ld\n",sizeof(buf[i]),strlen(buf[i]));
		write(fd,buf[i],strlen(buf[i]));
	}
	close(fd);
}
