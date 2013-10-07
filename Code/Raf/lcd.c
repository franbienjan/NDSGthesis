#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include "rabin_polynomial.h"
#include "rabin_polynomial_constants.h"

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
    #define PLATFORM 0
#elif __unix__
    #define PLATFORM 1
#elif __linux__
    #define PLATFORM 2
#elif __APPLE__
    #define PLATFORM 3
#endif

// to compile: gcc rabin_polynomial.c lcd.c sha1.c

// [] Implement on one folder.
// [] Implement for multiple folders w/in File Agent.
// [] Write "future" todo list.
// [] 0 filesize hangs!!! D:
// [] how to know if file has already been chunked?!?!?! (maybe some file from somewhere? Master Server?)
// [] cleanup code: separate rabin from chunking for more flexibility
// [] Place lcd w/in File Agent okay? (do necessary directory edits :( )
// [] Possible problem: SHA-1 file-reading. (EOF?)
// [] RESTORE FILES FROM CHUNKS
// [] lolololol
// [] when less than 4kb, do not chunk :|

char *file_agent_dir = "./";
char *file_agent_dir_system = "\"File Agent\"\\";
char *file_storage_dir = "./Storage/";
char *chunk_storage_dir = "chunkstempofolder";

int chunk_storage_exists = 0;
int file_count = 0;

struct files{
	char filename[300];
	char soloname[100];
	char chunkID[50];
	char ptr_address[100];
	struct files* ptr;
};

struct files *root;



void getFileNames(){

    // [x] ignore multimedia/compressed files
    
	// FOR WINDOWS
	DIR *dir;
	FILE *raf;
	struct dirent *ent;
	struct files *temp;
	char nondupes[300],tsplit[300],nsplit[150],*splithash[10],*splitpath[20];
	int i = 0, j = 0;
	
	raf = fopen("./out.txt","r");
	//raf = fopen("./uniquefiles.txt","r");
	
	
	root = (struct files *)malloc(sizeof(struct files));
	temp = root;
	
	while (fgets(nondupes, 300, raf) != NULL) {
		i = 0;
		j = 0;
		
	// split accdg "|"; get filepath
		splithash[i] = strtok(nondupes, "|");
		while (splithash[i] != NULL){
			splithash[++i] = strtok(NULL, "|");
		}
		strcpy(tsplit,splithash[0]);
		
	// split accdg "."; get filetype
		splitpath[j] = strtok(splithash[0], ".");
		while (splitpath[j] != NULL){
			splitpath[++j] = strtok(NULL, ".");
		}
		
		
	// save select names into RAM aka disregard ff filetypes
		//printf("splitpath: %s\n",splitpath[j-1]);
		if(strcmp(splitpath[j-1],"DS_Store") && strcmp(splitpath[j-1],chunk_storage_dir) && strcmp(splitpath[j-1],"bmp") && strcmp(splitpath[j-1],"png") && strcmp(splitpath[j-1],"jpg") && strcmp(splitpath[j-1],"jpeg") && strcmp(splitpath[j-1],"gif") && strcmp(splitpath[j-1],"zip") && strcmp(splitpath[j-1],"rar") && strcmp(splitpath[j-1],"tar") && strcmp(splitpath[j-1],"tiff") && strcmp(splitpath[j-1],"svg") && strcmp(splitpath[j-1],"mp3") && strcmp(splitpath[j-1],"mp4") && strcmp(splitpath[j-1],"mkv") && strcmp(splitpath[j-1],"exe")){


			strcpy(temp->filename,tsplit);
		
		
		// split accdg "."; get filename
			i = 0;
			splithash[0] = strtok(splitpath[j-2], "/");
			while (splithash[i] != NULL){
				splithash[++i] = strtok(NULL, "/");
			}
			strcpy(nsplit,splithash[i-1]);
			strcat(nsplit,".");
			strcat(nsplit,splitpath[j-1]);\
			strcpy(temp->soloname,nsplit);
			//printf("splithash: %s\n",temp->soloname);
			
			file_count += 1;
			temp->ptr = (struct files *)malloc(sizeof(struct files));
			temp = temp->ptr;
		}
		
	}
	
	fclose(raf);
	
	
	
	// look in given directory
	if ((dir = opendir (file_agent_dir)) != NULL) {
	
		// loop through each file in directory
		while ((ent = readdir (dir)) != NULL) {
			
			// check if chunk directory exists
			if(!strcmp(ent->d_name,chunk_storage_dir)){
				chunk_storage_exists = 1;
			}
		}
		closedir (dir);
	}
	else {
		perror ("");
		//return EXIT_FAILURE;
	}
	
	
	// check if filenames are saved
	/*
	temp = root;
	for(i = 0 ; i < file_count; i++){
		printf("%s\n",temp->filename);
		temp = temp->ptr;
	}
	*/
	
}








void rabinChunking(){

	// CONSTANTS: (ESTIMATES FOR SAM)
	// RABIN WINDOW - 31 BYTES
	// RABIN AVG BLOCK - 8KB (8192 bytes)
	// RABIN TEMP MIN - 4KB (4096 bytes)
	// RABIN TEMP MAX - 16KB (16384 bytes)

	// [x] make chunk directory
	// [x] read files
	// [x] rabin algorithm
    // [x] chunk
    // [x] save chunks in a (hidden) folder w/in containers
    // [x] SHA-1 function to compute for chunk IDs
	
	// [x] determine primary fingerprint // HOW TO GET MINIMUM HASH?!?!? -- get minimum value. durr.
    // [x] primary/representative IDs must contain pointers to bin
	// [x] bin contains chunk ID, chunk sizes, and chunk addresses (.txt file, maybe?)
    // (one bin per file!)
    // [x] determine secondary fingerprint
    // [x] dump into disk
	// [x] piggyback small files

	
	char concat[300],**chunknames;
	struct files *temp;
	FILE *fin,*fout;
	int i;
	
	temp = root;
	
	rabin_polynomial_min_block_size = 4096;
	rabin_polynomial_max_block_size = 16384;
	rabin_polynomial_average_block_size = 8192;
	
	
	
	// loop through each file to get fingerprint
	for(i=0; i<file_count; i++){
		// create chunk storage directory if it does not exist yet
		if(chunk_storage_exists == 0){
			if(PLATFORM == 0){
				strcpy(concat,"md .\\"); //////////edit later file_agent_dir_system
			}
			else{
				strcpy(concat,"mkdir ./");
			}
			strcat(concat,chunk_storage_dir);
			system(concat);
			memset(concat, 0, sizeof concat);
		}
		
		// read a particular file
		printf("file read: %s\n",temp->filename);
		fin = fopen(temp->filename, "rb+"); // made a code change here!!! initially r+ :)
		if(fin == NULL) {
			fprintf(stderr, "Cannot read file %s!\n",temp->filename);
		}
		
		// rabin algorithm + chunking
		struct rabin_polynomial *head = get_file_rabin_polys(fin);
		fclose(fin);
		free_rabin_fingerprint_list(head);
		
		// prints bin in stdout
		fin = fopen(temp->filename, "rb");
		
		strcpy(concat,file_agent_dir);
		strcat(concat,chunk_storage_dir);
		strcat(concat,"/");
		
	//	printf("concat: %s\n",concat);
        chunknames = print_rabin_poly_list_to_file(fout,head,concat,fin); // [] modify to return array of chunknames per 1 file!!! , number of chunks == sizeof / length
		
	//	printf("MINDEX = %d\nMIN HASH = %s\nchunkcount = %d\n",mindex,minimum_sha,chunkcount);
    
    
    
	// EXTREME BINNING
        
		// piggyback small files; no need to bin
		int cc = chunkcount + 1;
		if(cc <= 2){
			char holder[100];
			strcpy(holder,chunknames[0]);
			if(cc == 2){
				strcat(holder,"_");
				strcat(holder,chunknames[1]);
			}
			strcpy(temp->chunkID,holder);
		}
		// normal files; must bin
		else{
			FILE *bin;
			int k = 0;
			
			strcpy(temp->chunkID,minimum_sha);
			
			strcat(concat,"__bin__");
		//	strcat(concat,temp->soloname);
		//	strcat(concat,"__");
			strcat(concat,temp->chunkID);
			strcat(concat,".bin");
		//	printf("concat: %s\n",concat);
			bin = fopen(concat, "w");
			
			fprintf(bin,"%s\n",temp->filename);
			for(k=0;k<cc;k++){
				if(strcmp(temp->chunkID,chunknames[k])){ // if not primary
					fprintf(bin,"%s\n",chunknames[k]); //might add chunk size later? (but it's pretty useless though)
				}
			}
			fclose(bin);
		}
    
    
	
		// progress to next file
		temp = temp->ptr;
		memset(concat, 0, sizeof concat);
    
    
		
	}
	
	
}






// to do:
// [x] check SHA-1
// [x] determine minimum hash (consider other data-passing methods)
// [x] ignore small file sizes
// [x] write secondary hashes in file
// [x] compare primary hashes
// [] compare secondary hashes // kahit wag na muna 'to (automatic)





void comparePrints(){
	// [x] compare primary fingerprints
    // [] if equal, compare secondary fingerprints
    // [] else, skip
    
    int i = 0, j = 0, k = 0;
    char mdir[150],tdir[150];
	struct files *file_array[file_count];
	struct files *temp;
	temp = root;
    
    strcpy(mdir,file_agent_dir);
    strcat(mdir,chunk_storage_dir);
    strcat(mdir,"/");
	
	// linked list -> array
    for(i=0;i<file_count;i++){
		file_array[i] = temp;
		temp = temp->ptr;
	}
	
	/*
	
	for(i=0;i<file_count;i++){
		for(j=file_count-1;j>i;j--){
			if(!strcmp(file_array[i]->chunkID,file_array[j]->chunkID)){
				// compare written
                //
                // comment out
                fopen(tdir, "r");
                
			}
		}
	}
	*/
	
}






void remakeFiles(){
    
}




int main(){
    
	// get contents of file agent
    getFileNames();
    
	// rabin fingerprinting, SHA-1, extreme binning
    rabinChunking();
    
    // compare chunks
    comparePrints();
    
    // remake new files from chunks
    remakeFiles();
    
	
	return 0;
    
}