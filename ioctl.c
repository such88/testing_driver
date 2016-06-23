#include "chardev.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>

ioctl_set_msg(int file_des, char *msg)
{
	int ret_val;
	ret_val = ioctl(file_des, IOCTL_SET_MSG, msg);
	
	if(ret_val<0)
	{
		printf("ioctl_set_msg failed: %d\n",ret_val);
		exit(-1);
	}
}

ioctl_get_msg(int file_des)
{
	int ret_val;
	char message[100];
	ret_val = ioctl(file_des, IOCTL_GET_MSG, message);
	
	if(ret_val<0)
	{
		printf("ioctl_get_msg failed: %d\n",ret_val);
		exit(-1);
	}

	printf("get_msg message: %s\n ", message);
}

ioctl_get_nth_byte(int file_des)
{	int i = 0;
	char c;
	char message[100]="ebar bol";
	puts("get nth byte");

	do{
		c = ioctl(file_des,IOCTL_GET_NTH_BYTE,i++);
printf("value of i: %d", i);
		
		if(c<0)
		{
			printf("ioctl nth byte failed at %d\n",i);
			exit(-1);
		}

		putchar(c);
	}while(c!=0);
	putchar('\n');
	
}

main()
{
	int file_desc, ret_val;
	char *msg = "Message passed by ioctl\n";
	char file[25];
	strcpy(file,"/dev/");
	strcat(file,DEVICE_FILE_NAME); 
	file_desc = open(file,0);
	if(file_desc<0)
	{
		printf("can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}

	ioctl_get_nth_byte(file_desc);
	ioctl_get_msg(file_desc);
	ioctl_set_msg(file_desc,msg);
	
	close(file_desc);
}
