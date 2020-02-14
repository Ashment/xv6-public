#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
uniq(int fd, char *name){

	int i, n;
	printf(1,"Starting Uniq...\n");

	while((n=read(fd, buf, sizeof(buf))) > 0){
		
		char matching = 1;
		char* currLine = malloc(1024 * sizeof(char));
		int c = 0; // Counts length of current line to expand line buffer if needed.
		char* lastLine;

		for(i=0; i<n; i++){
			// Read the next character
			char nChar = buf[i];

			if(nChar == '\n'){
				// End of line.
				currLine[c] = '\0';
				if(!matching){
					printf(1, "%s\n", lastLine);
					lastLine = currLine;
				} 
				free(currLine);
				currLine = malloc(1024 * sizeof(char));
				c = 0;

			}else{
				// Next character read
				currLine[c] = nChar;
				c = c + 1;
/*
				if(c%128 == 0){
					// Expand the string size where necessary.
					currLine = (char*) realloc(currLine, (c+128) * sizeof(char));
				}
*/
			}
		}

	}
	// End of file
}

int 
main(int argc, char *argv[]){

	int fd;

	printf(1, "ARGC: %d\n",argc);

	if(argc == 2){
		if((fd = open(argv[1],0)) < 0){
			printf(1, "uniq: cannot open %s\n", argv[1]);	
		}else{
			uniq(fd, argv[1]);
		}
	}else{
		printf(1, "More than one argument found. Not supported.");
		exit();	
	}

	exit();

}

