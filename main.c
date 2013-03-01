#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hidapi.h"
#include <stdio.h>
#include <stdlib.h>

char *ids_file;

struct node *root;

// write id to logfile
void write_logfile(char *input){
    FILE *fp;
    fp = fopen((ids_file == NULL) ? "ids.txt" : ids_file, "a+");
    fprintf(fp, "%s\n", input);
    fclose(fp);
}


struct node {
    struct node *next;
    unsigned long uid;
    char fname[50];
    char email[50];
    char lname[50];
};
#define LINESZ 1024

struct node *readin(){
    FILE *fp;
    int ok = 1;
    struct node *cur;
    struct node *root;
    struct node *last;
    root = last;
    char buf[LINESZ];

    fp = fopen("ids_map.txt", "r");

    if (fp != NULL) {
        while (fgets (buf, LINESZ, fp)) {
            cur = malloc(sizeof(struct node));
            cur->next = NULL;
            if (sscanf(buf, "%s\t%s\t%s\t%lu\n", cur->fname, cur->lname, cur->email, &cur->uid) != EOF){
                if (!last){
                    last = root = cur;
                }
                last->next = cur;
                last = cur;
            }
        }
        fclose (fp);
    }
    return root;
}

void swipe(char *uscid){
    struct node *cur;
    unsigned long search = atol(uscid);
    cur = root;
    char buf[300];
    do {
        if (cur->uid == search){
            printf("%s %s\n", cur->fname, cur->lname);
	    sprintf(buf,"%lu,%s,%s", cur->uid, cur->fname, cur->lname);
	    write_logfile(buf);
	    return;
        }
    } while(cur = cur->next);
    write_logfile(uscid);
};


int main(int argc, char* argv[])
{
    #define MAX_BUF 200
    unsigned char buf[MAX_BUF];
    unsigned char *bufp;
    unsigned char userid[12];
    hid_device *handle;
    int i, n, res;

    root = readin();

    if (argc > 1){
        ids_file = argv[1];
        printf("Using ID file: %s\n", argv[1]);
    } else {
        ids_file = NULL;
        printf("Using ID file: ids.txt\n");
    }

    // open magtek    reader  magtekID
    handle = hid_open(0x0801, 0x0002, NULL);

    while(1){
        // Read requested state
        res = hid_read(handle, buf, MAX_BUF);
        if (res < 0){
            printf("Unable to read data.\n");
            break;
        }

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
            swipe(userid);
        } else {
            printf("\tInvalid card read!\n");
        }  
    }

    return 0;
}
