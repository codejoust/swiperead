#include "reader.c"

void onSwipe(unsigned long uid){
	printf("Got userid: %lu\n", uid);
}

void onError(){
	printf("Error reading card!\n");
}

int main(int argc, char *argv[]){
	
	handle_swipes(&onSwipe, &onError);

}