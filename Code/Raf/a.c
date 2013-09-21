#include <time.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
//#include <sys/types.h>
//#include <unistd.h>
#include <sys/stat.h>
#include<dirent.h> // for dir algo
#include "sha1.h"

// searches the string 'str' in file 'fname'
int searchWord(char *fname, char *str) ;

// prints out the changed files and puts in out.txt
void compareFiles();

// initializes the file list in gg.txt
void initFileList();

// returns the size of the file in bytes
off_t getSize(const char *filename);

// determines if the file in string 'target' is a folder
int isDir(const char* target);

// test function for getSha()
void test(int err, const char* msg);

// gets "sha-1"
char* getSha(char* fileName, int fileSize);

/*
 *	gg.txt - information of files
 *  out.txt - list of files to be duplicated
 *
 */
 
typedef struct FolderList{
	struct FolderList *next;
	DIR *dir;
}FolderList;

void main() {
	int i;
	FILE *fp;
	do{
		printf("1 to initialize txt file\n2 to compare\n3 to exit\n:");
		scanf("%d",&i);
		if(i==1) initFileList();
		else if(i==2) compareFiles();	
		else if(i==3){
			break;
		}
	}while(1);
}

void initFileList(){
	DIR *dir;
	char t[ 100 ] = "";
	struct dirent *ent;
	struct stat b;
	FILE *f;
	f=fopen("gg.txt","w+");
	
	//if ((dir = opendir ("C:\\Users\\Raf\\Desktop\\NDSGthesis\\Code\\Raf")) != NULL) {
	if ((dir = opendir (".")) != NULL) {
		printf("Initialized files:\n");
		while ((ent = readdir (dir)) != NULL){
			// if the file is a folder
			if(isDir(ent->d_name)==1) ;
			
			// excludes unnecessary scans
			else if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0 && strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.exe")!=0 && strcmp(ent->d_name,"gg.txt")!=0 && strcmp(ent->d_name,"out.txt")!=0 && strcmp(ent->d_name,"sha1.c")!=0 && strcmp(ent->d_name,"sha1.h")!=0){
				
				if (!stat(ent->d_name, &b)){
					// get file name
					fprintf(f, "%s|", getSha(ent->d_name,getSize(ent->d_name)));
					printf("%s|",getSha(ent->d_name,getSize(ent->d_name)));
					
					// get file size
					fprintf(f, "%d|",getSize(ent->d_name));
					printf("%d|",getSize(ent->d_name));
				
					//get date created
					strftime(t, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_ctime));
					fprintf(f, "%s|",t);
					printf("%s|",t);
					
					//get date modified
					strftime(t, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_mtime));
					fprintf(f, "%s|",t);
					printf("%s|",t);
					
					//get date accessed
					strftime(t, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_atime));
					fprintf(f, "%s|",t);
					printf("%s|",t);
					
					fprintf(f, "%s\n",ent->d_name);
					printf("%s\n",ent->d_name);
				} else{
					printf("%s Cannot display the time.\n",ent->d_name);
				}
				
			} //endif
	  } //endwhile
	  closedir (dir);
	}else perror ("");
	fclose(f);
}

void compareFiles(){
	printf("\ncomparing..\n");
	DIR *dir;
	char t[ 100 ] = "";
	struct dirent *ent;
	struct stat b;
	FILE 	*f,
			*f2;
	f=fopen("gg.txt","r");
	f2=fopen("out.txt","w+");
	
	//if ((dir = opendir ("C:\\Users\\Raf\\Desktop\\NDSGthesis\\Code\\Raf")) != NULL) {
	if ((dir = opendir (".")) != NULL) {
		while ((ent = readdir (dir)) != NULL){
			// excludes unnecessary scans
			if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0 && strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.exe")!=0 && strcmp(ent->d_name,"gg.txt")!=0 && strcmp(ent->d_name,"out.txt")!=0  && strcmp(ent->d_name,"sha1.c")!=0 && strcmp(ent->d_name,"sha1.h")!=0){
				
				// prints date of modification
				if (!stat(ent->d_name, &b)){
					
					if(searchWord("gg.txt",ent->d_name)!=0){
					
						char buffer[256];
						strcpy(t, getSha(ent->d_name,getSize(ent->d_name)));
						strcat(t,"|");
						strcat(t,itoa(getSize(ent->d_name),buffer,10));
						strcat(t,"|");
						strftime(buffer, 100, "%m%d%Y-%H:%M:%S|", localtime(&b.st_ctime));
						strcat(t,buffer);
						strftime(buffer, 100, "%m%d%Y-%H:%M:%S|", localtime(&b.st_mtime));
						strcat(t,buffer);
						strftime(buffer, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_atime));
						strcat(t,buffer);
						printf("searching for\n%s\n",t);
						if(searchWord("gg.txt",t)==0){
							
							printf("\nfilename:%s\nfile '%s' is changed\n\n",t,ent->d_name);
							fprintf(f2, "%s\n",t);
						}
					}
					
				} else printf("Cannot display the time.\n");

			} //endif
		} //endwhile

	  closedir (dir);
	}else perror ("");
	fclose(f);
	fclose(f2);
	initFileList();
}

int searchWord(char *fname, char *str) {
	FILE *f;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	
	if((f = fopen(fname, "r")) == NULL) return(-1);

	while(fgets(temp, 512, f) != NULL) {
		if((strstr(temp, str)) != NULL) {
			//printf("A match found on line: %d\n", line_num);
			//printf("\n%s\n", temp);
			find_result++;
			//printf("1\n");
			return 1;
		}
		line_num++;
	}

	if(find_result == 0) {
		//printf("Sorry, couldn't find a match.\n");
		//printf("0\n");
		return 0;
	}
	
	//Close the file if still open.
	if(f) fclose(f);
	
   	return 0;
}

off_t getSize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

int isDir(const char* target){
   struct stat statbuf;
   stat(target, &statbuf);
   return S_ISDIR(statbuf.st_mode);
}

void test(int err, const char* msg) {
    if (err) {
        fprintf(stderr, "%s: error %d\n", msg, err);
        exit(1);
    }
}

char* getSha(char* fileName, int fileSize){
	char split[20];
	char* eureka=malloc(50);
    int n, i, len;
    static unsigned char buf[20] = {0};
	
	char string[fileSize];
	FILE *fp;
	fp=fopen(fileName,"rb");
	if(!fp){
		printf("\nNo file found");
		return;
	}
	fread(string,1,fileSize,fp);

	SHA1Context sha;

    memset(eureka, 0, sizeof eureka);

    // Print a hash of the key
    len = strlen(string);  // key length
    test(SHA1Reset(&sha), "SHA1Reset");
    test(SHA1Input(&sha, string, len), "SHA1Input");
    test(SHA1Result(&sha, buf), "SHA1Result");
    //printf("Key hash = ");
    for (i=0; i<20; ++i){
        sprintf(split, "%02X", buf[i]);
        strcat(eureka,split);
    }
	return eureka;	
}