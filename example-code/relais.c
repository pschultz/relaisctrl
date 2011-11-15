/* relais example code
 * Copyright (C) 2003	Joachim Schiele 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *	-rwxr-xr-x    1 joachim  users        2612 Mar 13 16:38 relais.c
 *
 */

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 *
 * RS232-standard 
 * - 19200 baud
 * - 8 bits/byte
 * - no parity
 * - no handshake
 * - 1 stop bit
 */

/*
 * port can also bey /dev/ttyS1
 * if you don't run this as root check permissions
 * chmod 0777 /dev/ttyS0 should give every user read/exec/write permission
 * but BECARFEFUL with this setting it's a security risc.
*/
#define PORT "/dev/ttyS0"
#define STDIN 0
typedef unsigned char uchar;

/*
 * this function helps us to set the port ...
 * it generates the XOR for our command checksum!!
*/
int sndcmd(uchar a, uchar b, uchar c, int fd) {
	unsigned char wbuf[3];
	wbuf[0] = a;
	wbuf[1] = b;
	wbuf[2] = c;
	wbuf[3] = wbuf[0]^wbuf[1]^wbuf[2];

	printf(" -> send: %d %d %d %d\n", a,b,c, wbuf[3]);
	if (write(fd, wbuf, 4)) 
		return 1;
	else
		return -1;
}

int main(void)
{
  int fd; /* File descriptor for the port */
	int n;
	unsigned char buf[3];
	unsigned char rbuf[3];

        unsigned char keyBuf[1];

	struct termios options;

	fd = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1)
  {
    /*
     * Could not open the port.
    */

    perror("open_port: Unable to open /dev/ttyS?");

  } else {
    fcntl(fd, F_SETFL, 0);
		
		/* get the current options */
		tcgetattr(fd, &options);
	
		/* set in and out speed */
		cfsetispeed(&options, B19200);
		cfsetospeed(&options, B19200);
	
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~CSIZE; /* Mask the character size bits */
		options.c_cflag |= CS8;    /* Select 8 data bits */
	
		/* set raw input, 1 second timeout */
		options.c_cflag     |= (CLOCAL | CREAD);
		options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
		options.c_oflag     &= ~OPOST;
		options.c_cc[VMIN]  = 0;
		options.c_cc[VTIME] = 10;
	
		/* set the options */
		tcsetattr(fd, TCSAFLUSH, &options);

		/*
		 * first we init the circuit board
		*/
		sndcmd(1, 1, 0, fd);
		fcntl(fd, F_SETFL, FNDELAY);
		fcntl(fd, F_SETFL, 0);
		read(fd, rbuf, 4);
		printf("%d %d %d %d\n", rbuf[0], rbuf[1], rbuf[2], rbuf[3]);

		/*
		 * for test purpose we set all relais with 255 as you can see
		*/
		sndcmd(3, 0, 255, fd);
		read(fd, rbuf, 4);
		printf("%d %d %d %d\n", rbuf[0], rbuf[1], rbuf[2], rbuf[3]);
                printf("All LEDs should be red now\nPress any key to continue\n");
                read(STDIN, keyBuf, 1);

		sndcmd(3, 0, 0, fd);
		read(fd, rbuf, 4);
		printf("%d %d %d %d\n", rbuf[0], rbuf[1], rbuf[2], rbuf[3]);
	}

	close(fd);
  return 1;
}

