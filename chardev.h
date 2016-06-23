#ifndef CHARDEV_H
#define CHARDEV_H
#endif

#include<linux/ioctl.h>

#define MAJOR_NUM 240

#define IOCTL_SET_MSG _IOR(MAJOR_NUM,0,char *)
/* _IOR means that we are creating IOCTL comamnd number
 * for passing infoformation from user process to kernel module
 *
 * 1st argument device MAJOR_NUM, 2nd argument no of command, 3rd command
 * is type we want to get from the the process to the kernel
 */

#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
/* this IOCTL used for the output, to get the message of device driver
 */

#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)
/* the IOCTL used for both input and output. It receives from the user a number, 
* n, and returns Message[n]
*/

#define DEVICE_FILE_NAME "char_dev"


