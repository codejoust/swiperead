#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hidapi.h"
#include <stdio.h>

// write id to logfile
void write_logfile(char *input){
	FILE *fp;
	fp = fopen("ids.txt", "a+");
    fprintf(fp, "%s\n", input);
    fclose(fp);
}

int main(int argc, char* argv[])
{
	int res;
	#define MAX_BUF 240
	unsigned char buf[MAX_BUF];
    unsigned char *bufp;
	unsigned char userid[12];

	hid_device *handle;
	int i, n;

	// open magtek    reader  magtekID
	handle = hid_open(0x0801, 0x0002, NULL);

	while(1){
		// Read requested state
		res = hid_read(handle, buf, 240);
		if (res < 0)
			printf("Unable to read data.\n");
			break;

		printf("Data read:\n");
		res = 0;
		bufp = buf;
        i = 0; n = 0;
	
        while((++i < MAX_BUF) && '%' != *bufp){
        	bufp++;
        }

        while(*bufp != '?' && ++i < MAX_BUF){
        	bufp++;
        	if (*bufp >= '0' && *bufp <= '9' && n < 11){
				userid[n++] = *bufp;
        	}
        }
        userid[n++] = '\0';

		if (n > 1){
			printf("\tSwiped User ID: %s\n", userid);
			write_logfile(userid);
		} else {
			printf("\tInvalid card read!\n");
	    }  
	}

	return 0;
}
