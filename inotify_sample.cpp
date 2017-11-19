#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

/*
	header file to be included for the 
	inotify api

	this program will run to check whether 
	a watch files in the configuration file
	and if a proposed set of events 
	occired on the file being watched 
	it gives the notification
	
	program for watchig 100 files 
	simulatanoeusly

*/


#include <sys/inotify.h>
#include <limits.h>

/*
	A buffer big enough to read 100 events
	in one gos
*/

#define BUFSIZE (100*(sizeof(struct inotify_event)+NAME_MAX+1));

int main(){
	
	FILE *f_config = NULL;/*descriptor for the configuration file*/
	FILE *f_out    = NULL;/*descriptor for the output file*/

	/*
		inotify api descriptors
	*/

	int i_notify_fd = -1;
	int watch_fd    = -1;

	/*
		Array for holding the watched file names
		indexed by the watch descriptor
	*/
	
	char watch_name[PATH_MAX];
	char watched_file_names[100][NAME_MAX+1];

	/*
		intializing the inotify api
	*/

	i_notify_fd = inotify_init();
	if(i_notify_fd < 0){
		perror("inotify_init");
		exit(0);
	}

	/*
		open the config file for 
		reading the filenames
	*/

	f_config = fopen("monitor.conf","r");
	if(f_config == NULL){
		perror("fopen");
		exit(0);
	}

	/*
		read all tje watched file 
		names from the config file
	*/


	return 0;
}
