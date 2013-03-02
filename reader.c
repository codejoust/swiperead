#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hidapi.h"
#include <stdio.h>

#define MAX_BUF 300
typedef unsigned long idt;


void handle_swipes(void (*onSwipe)(idt uid), void (*onError)())
{
    unsigned char buf[MAX_BUF];
    unsigned char *bufp;
    unsigned char userid[12];
    hid_device *handle;
    int i, n, res;

    // open magtek    reader  magtekID
    handle = hid_open(0x0801, 0x0002, NULL);

    while(1){
        // Read requested state
        res = hid_read(handle, buf, MAX_BUF);
        if (res < 0){
        	onError();
            break;
        }

        printf("Data read:\n");
        res = 0;
        bufp = buf;
        i = 0; n = 0;

        buf[MAX_BUF-1] = '\0';
        int i = 0;
        while((++i < MAX_BUF) && ';' != *bufp){
            bufp++;
        }
        bufp++;
        while((++i < MAX_BUF) && ';' != *bufp){
            bufp++;
        }

        while(*bufp != '=' && ++i < MAX_BUF){
            bufp++;
            if (*bufp >= '0' && *bufp <= '9' && n < 11){
                userid[n++] = *bufp;
            }
        }
        userid[n++] = '\0';

        if (n > 1){
            onSwipe(atol(userid));
        } else {
        	onError();
        }  
    }

}
