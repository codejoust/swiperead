PATH_TO_HIDAPI=../hidapi/

cardread: main.c
	gcc -I$(PATH_TO_HIDAPI)/hidapi/ -L$(PATH_TO_HIDAPI)/mac/ -lhidapi main.c  -o cardread
