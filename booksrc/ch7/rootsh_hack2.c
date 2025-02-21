/*
 * ch7/rootsh_hack2.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 7 : Process Credentials
 ****************************************************************
 * Brief Description:
 * For details, please refer the book, Ch 7.
 */

/*
Order of execution is important here b/c changing the owner resets the execute bit to 'x'.
After running 'make', run the following two commands: 
	1) sudo chown root ./rootsh_hack2
	2) sudo chmod u+s rootsh_hack2
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

int main(int argc, char **argv)
{	
	// query real, effective, and saved-setuid
	uid_t ruid, euid, suid;
	getresuid(&ruid, &euid, &suid);
	printf("ruid=%u, euid=%u, suid=%u\n", ruid, euid, suid);

	if (argc > 1) {
		/* Become root */
		if (setuid(0) == -1) {
			perror("setuid(0) failed!\n");
			//WARN("setuid(0) failed!\n");
			exit(EXIT_FAILURE);
		}
		getresuid(&ruid, &euid, &suid);
		printf("ruid=%u, euid=%u, suid=%u\n", ruid, euid, suid);
		setuid(1000);
		getresuid(&ruid, &euid, &suid);
		printf("ruid=%u, euid=%u, suid=%u\n", ruid, euid, suid);
	}

	/* 
	Now just spawn a shell; 
	<i>Evil Laugh</i>, we're now root! 
	*/
	 
	system("/bin/bash");
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
