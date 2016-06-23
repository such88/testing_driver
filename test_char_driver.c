#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<assert.h>


int main(int argc, char **argv)
{
	int fd;	
	assert(argc > 1);
	char buf[100];
	int i = 0;
	char str[100];
	printf("Enter value is %s...  \n",argv[1]);
	memset(buf,0,100);

//	scanf("%s",str);
//	printf("Enter value is %s...  ",argv[1]);
	if((fd = open("/dev/char_dev",O_RDWR))<0)
		printf("error opening file");
	else
		printf("opening file");
	write(fd, argv[1], strlen(argv[1]));
	while(read(fd, &buf[i++], 1));

	printf("Reserved by the driver: %s\n", buf);
}
