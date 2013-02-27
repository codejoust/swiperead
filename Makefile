PATH_TO_HIDAPI=../hidapi/
all: basicread readmap
basicread: basicread.c reader.c
	gcc -g -I$(PATH_TO_HIDAPI)/hidapi/ -L$(PATH_TO_HIDAPI)/mac/ -lhidapi basicread.c -o basicread 
readmap: readmap.c reader.c
	gcc -g -I$(PATH_TO_HIDAPI)/hidapi/ -L$(PATH_TO_HIDAPI)/mac/ -lhidapi readmap.c  -o readmap 
