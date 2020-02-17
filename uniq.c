#include "types.h"
#include "stat.h"
#include "user.h"
#include "ctype.h"

char buf[512];

void
uniq(int fd, char *name, char* ops[]){

	int i, j, n;
	int lCount = 1;
	char opC = 0;
	char opD = 0;
	char opI = 0;

	if(ops){ 
		// If there are options, read them and set the corresponding option flags.
		for(i=0; i<sizeof(4); i++){		
			if(strcmp("-c", ops[i]) == 0){
				opC = 1;
				//printf(1, "OPS >> C", ops[i]);
			}else if(strcmp("-d", ops[i]) == 0){
				opD = 1;
				//printf(1, "OPS >> D", ops[i]);
			}else if(strcmp("-i", ops[i]) == 0){
				opI = 1;
				//printf(1, "OPS >> I", ops[i]);
			}
		}
	}


	char matching = 1;
	char* currLine = (char *)malloc(128 * sizeof(char));
	int c = 0; // Counts length of current line to expand line buffer if needed.
	char* lastLine = (char *)malloc(sizeof(char));

	while((n=read(fd, buf, sizeof(buf))) > 0){
		
		for(i=0; i<n; i++){
			// Read the next character
			char nChar = buf[i];

			if(nChar == '\n'){
				// End of line.
				currLine[c] = '\0';
				if(lastLine[c]){
					matching = 0;
				}
				if(!matching){
					free(lastLine);
					lastLine = currLine;
					if(!opD){
						if(opC){
							printf(1, "%d ", lCount);
						}
						printf(1, "%s\n", currLine);
					}
					lCount = 1;
				}else{
					lCount++;
					if(opD){
						if(opC){
								printf(1, "%d ", lCount);
						}
					printf(1, "%s\n", currLine);
					}
				}

				currLine = (char *)malloc(1024 * sizeof(char));
				c = 0;
				matching = 1;

			}else{
				// Next character read
				currLine[c] = nChar;
				if(opI){
					if(currLine[c] > 96){
						currLine[c] = currLine[c] - 32;
					}
				}

				if(matching){
					if(lastLine[c]){
						if(lastLine[c] != currLine[c]){
							matching = 0;
						}
					}else{
						matching = 0;
					}
				}

				c++;


				if(c%128 == 0){
					// Expand the string size where necessary.
					//currLine = (char*) malloc(currLine, (c + 128) * sizeof(char));
					char* tempLine = (char *) malloc((c+128) * sizeof(char));
					for(j=0; j<c; j++){
						tempLine[j] = currLine[c];
					}
					free(currLine);
					currLine = tempLine;
				}

			}
		}

	}
	// End of file
}

int 
main(int argc, char *argv[]){

	int fd, i;

	printf(1, "ARGC: %d\n", argc);

	if(argc == 2){
		if((fd = open(argv[1],0)) < 0){
			printf(1, "uniq: cannot open %s\n", argv[1]);
		}else{
			char* blankOps[4];
			uniq(fd, argv[1], blankOps);
		}
	}else if(argc >= 2){
		if((fd = open(argv[argc - 1],0)) < 0){
			printf(1, "uniq: cannot open %s\n", argv[argc - 1]);
		}else{
			char* options[4];
			for(i=1; i<(argc - 1); i++){
				options[i] = argv[i];
				printf(1, "options -> %s\n", options[i]);
			}
			uniq(fd, argv[1], options);
		}
	}
	
	exit();

}

