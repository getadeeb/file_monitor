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
#include <unistd.h>
/*
	A buffer big enough to read 100 events
	in one gos
*/

#define BUFSIZE (100*(sizeof(struct inotify_event)+NAME_MAX+1))

int main(){
	
	FILE *f_config = NULL;/*descriptor for the configuration file*/
	FILE *f_out    = NULL;/*descriptor for the output file*/

	/*
		inotify api descriptors
	*/
	
	int i_notify_fd         = -1;
	int watch_fd            = -1;
	
	/*
		descriptor for reading the inotify 
		interface
	*/
	
	int inotify_no_of_events = -1;


	/*
		Array for holding the watched file names
		indexed by the watch descriptor
	*/
	
	char watch_name[PATH_MAX];
	char watched_file_names[100][NAME_MAX+1];

	/*
		Structure for fetching the file information
	*/

	struct stat sb;

	/*
		event buffer to hold all the
		events returned by the inotify api
	*/
	char event_buffer[BUFSIZE];

	/*
		event record pointer
	*/

	char *event_pointer = NULL;
	
	/*
		inotify event structure
	*/
	struct inotify_event *one_event = NULL;

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
		read all the watched file 
		names from the config file
	*/

	while(fgets(watch_name,PATH_MAX,f_config)){
		watch_name[strlen(watch_name)-1] = '\0';
		
		/*
			check whether the file exists or 
			can be accessed by this program
		*/

		int file_stat = stat(watch_name,&sb);
		if(file_stat < 0){
			printf("Cant stat %s, ignored\n",watch_name);
			continue;
		}

		/*
			if the file is a regular file 
			add it to the watch list
		*/

		if(S_ISREG(sb.st_mode)){
			watch_fd = inotify_add_watch(i_notify_fd,watch_name,IN_MODIFY | IN_DELETE_SELF);
			if(watch_fd < 0){
				printf("Error adding watch for %s\n",watch_name);
			}else{
				printf("Added %s to the watch list %d\n",watch_name,watch_fd);
				
				/*
					Store the watched file 
					descriptor on our array
				*/
				strcpy(watched_file_names[watch_fd],watch_name);
			}
		}else{
			/*
				probabaly this might
				 be a directory
			*/
			printf("%s in not a regular file,so ignored\n",watch_name);
		}	
	}

	/*
		right now we have parsed all the files in the descriptor
		to the memory area.
		now start wrrting the output to a file
	*/

	f_out = fopen("monitor.out","a");
	/*
		infinite loop to continously 
		monitor all the  events
	*/
		
	while(1){
		/*	
			returns the no of events
			happend on the files being watched
		*/
		inotify_no_of_events = read(i_notify_fd,event_buffer,BUFSIZE);
		
		/*
			loop throught all of the events
			and write it to the file buiffer
			here event records are not
			of fixed length so we have to iterate
			in a special way.
		*/

		for(event_pointer = event_buffer; 
			event_buffer < event_buffer + inotify_no_of_events;){
			one_event = (struct inotify_event *) event_pointer;
			
			/*
				if the event is modifying the 
				file,write it to the log file
			
			*/
			
			if(one_event->mask & IN_MODIFY){
				fprintf(f_out,"%s was modified\n",watched_file_names[one_event->wd]);
			}
			
			/*
				if the file is deleted
			*/

			if(one_event->mask & IN_DELETE_SELF){
				fprintf(f_out,"%s was deleted\n",watched_file_names[one_event->wd]);
			}
			
			/*
				flusing the file buffer writes everything
				to the file
			*/
			fflush(f_out);

		}
		fclose(f_out);	
	}

	return 0;
}
