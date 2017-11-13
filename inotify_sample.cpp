#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

/*
	header file to be included for the 
	inotify api
*/

#include <sys/inotify.h>
#include <limits.h>

/*
	A buffer big enough to read 100 events
	in one go.
*/

#define BUFSIZE (100*(sizeof(struct inotify_event)+NAME_MAX+1));

int main(){
	printf("this program is working");
	return 0;
}
