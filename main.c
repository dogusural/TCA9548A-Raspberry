#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <linux/i2c-dev.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

#define NUMBER_OF_DEVICES 8

int main(int argc, char **argv)
{


	// if ( argc != 2 )
	// {
	// 	printf("\n\r\t Usage ./binary [0-7]  \n\n\r");
	// 	return -1;
	// }

	// uint8_t input = **(argv+1) - 48;

	// if ( input > 8 )
	// {
	// 	printf("\n\r\t Usage ./binary [0-7]  \n\n\r");
	// 	return -1;
	// }

	char *filename = (char*)"/dev/i2c-1";
	char se_test[] = "./tests/v1_tests";
	char ses_test[] = "./tests/ses_tests";
 	int fd;

	if ((fd = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return -1;
	}

	int addr = 0x70;          //<<<<<The I2C address of the slave

	if (ioctl(fd, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return -1;
	}
	uint8_t buffer[1] = {0x00};
	for (size_t i = 0; i < NUMBER_OF_DEVICES; i++)
	{

		buffer[0] = (0x01 << i);

		size_t length = 1;			//<<< Number of bytes to write

		if (write(fd, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
		{
			/* ERROR HANDLING: i2c transaction failed */
			printf("Failed to write to the i2c bus.\n");
			return -1;
		}

		printf("\n\n\r\t *** Set-up the multiplexer for i2c device on the bus # %d *** \n\n\n\r",i);

		pid_t pid=fork();
		if (pid == -1)
		{
			exit(EXIT_FAILURE);
		}
		else if (pid==0)
		{ /* child process */
			static char *argv[]={"v1_tests",NULL,NULL};
			execv(se_test,argv);
			exit(127); /* only if execv fails */
		}
		else
		{ /* pid!=0; parent process */
			waitpid(pid,0,0); /* wait for child to exit */
		}

		pid=fork();
		if (pid == -1)
		{
			exit(EXIT_FAILURE);
		}
		else if (pid==0)
		{ /* child process */
			static char *argv[]={"ses_test",NULL,NULL};
			execv(ses_test,argv);
			exit(127); /* only if execv fails */
		}
		else
		{ /* pid!=0; parent process */
			waitpid(pid,0,0); /* wait for child to exit */
		}


	}


	close(fd);

	return 0;
}

