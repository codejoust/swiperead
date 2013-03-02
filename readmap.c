#include "reader.c"
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

struct node *readin(const char *ids_file){
    FILE *fp;
    int ok = 1;
    struct node *cur;
    struct node *root;
    struct node *last;
    root = last;
    char buf[LINESZ];

    fp = fopen(ids_file, "r");

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

void swipe(idt search){
    struct node *cur;
    char buf[300];
    cur = root;
    do {
        if (cur->uid == search){
            printf("%s %s\n", cur->fname, cur->lname);
            sprintf(buf, "%lu,%s,%s", cur->uid, cur->fname, cur->lname);
            write_logfile(buf);
            return;
        }
    } while(cur = cur->next);
    sprintf(buf, "%lu", search);
    write_logfile(buf);
};


void onSwipe(idt uid){
	printf("Got userid: %lu\n", uid);
	swipe(uid);
}

void onError(){
	printf("Error reading card!\n");
}

int main(int argc, char *argv[]){
	root = readin("ids_map.txt");
	handle_swipes(&onSwipe, &onError);
}

