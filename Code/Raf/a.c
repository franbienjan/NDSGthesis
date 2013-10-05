#include <time.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
//#include <sys/types.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "sha1.h"

// searches the string 'str' in file 'fname'
int searchWord(char *fname, char *str) ;

// gets the line in 'gg.txt' with the 'str' string
char* getLine(char *str,FILE *f);

// prints out the changed files and puts in out.txt
void compareFiles(const char *name, int level,FILE *f);

// initializes the file list in gg.txt
void initFileList(const char *name, int level,FILE *f);

// returns the size of the file in bytes
off_t getSize(const char *filename);

// test function for getSha()
void test(int err, const char* msg);

// gets "sha-1"
char* getSha(char* fileDirectory, char* fileName);

// initializes gg.txt
void callInit();
/*
 *	gg.txt - information of files
 *  out.txt - list of files to be deduplicated
 *
 */
 
typedef struct FolderList{
	struct FolderList *next;
	DIR *dir;
}FolderList;

FILE *file1;
FILE *file2;

void main() {
	int i;

	do{
		printf("1 to initialize txt file\n2 to compare\n3 to exit\n:");
		scanf("%d",&i);
		if(i==1){
			file1=fopen("gg.txt","w+");
			initFileList("./Galu",0,file1);
			fclose(file1);
		}else if(i==2){
			file1=fopen("gg.txt","r");
			file2=fopen("out.txt","w+");
			compareFiles("./Galu",0,file1);
			fclose(file1);
			fclose(file2);
			callInit();
		}else if(i==3){
			break;
		}
	}while(1);
}


void initFileList(const char *name, int level,FILE *f){
    DIR *dir;
    struct dirent *ent;
	char path[1024]="";
	char t[ 512 ] = "";
	int len;

	struct stat b;

    if (!(dir = opendir(name))) return;
    if (!(ent = readdir(dir))) return;
    int i=0;
    do{
    	// if folder
        if (ent->d_type == DT_DIR){

			if(strcmp(ent->d_name,"..")==0) len = snprintf(path, sizeof(path)-1, "%s", name);
			else len = snprintf(path, sizeof(path)-1, "%s/%s", name, ent->d_name);
            path[len] = 0;

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            initFileList(path, level + 1,f);
        }
        //if file
        else{
        	len = snprintf(path, sizeof(path)-1, "%s", name);
        	if(strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.out")!=0 && strcmp(ent->d_name,"sha1.c")!=0 && strcmp(ent->d_name,"sha1.h")!=0 && strcmp(ent->d_name,"out.txt")!=0 && strcmp(ent->d_name,"gg.txt")!=0) {
        		
        		char file[strlen(path)+strlen(ent->d_name)+1];
        		strcpy(file,path);
        		strcat(file,"/");
        		strcat(file,ent->d_name);

				stat(file, &b);
        		//printf("%s|",getSha(path,ent->d_name));
        		fprintf(f, "%s|", getSha(path,ent->d_name));

        		//printf("%lu|",getSize(file));
        		fprintf(f, "%lu|", getSize(file));

        		strftime(t, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_ctime));
        		//printf("%s|",t);
        		fprintf(f, "%s|",t);

        		strftime(t, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_mtime));
        		//printf("%s|",t);
				fprintf(f, "%s|",t);

				strftime(t, 100, "%m%d%Y-%H:%M:%S|", localtime(&b.st_atime));
				//printf("%s",t);
				fprintf(f, "%s",t);

				//printf("%s\n",file);
				strcpy(file,path);
        		strcat(file,"/\"");
        		strcat(file,ent->d_name);
        		strcat(file,"\"");
				fprintf(f, "%s\n",file);

        	}	
        }

    } while (ent = readdir(dir));
    
    closedir(dir);
}


void compareFiles(const char *name, int level,FILE *f){
    DIR *dir;
    struct dirent *ent;
	char path[1024]="";
	char t[ 512 ] = "";
	int len;

	struct stat b;


    if (!(dir = opendir(name))) return;
    if (!(ent = readdir(dir))) return;
    int i=0;

    do{
    	// if folder
        if (ent->d_type == DT_DIR){
			if(strcmp(ent->d_name,"..")==0) len = snprintf(path, sizeof(path)-1, "%s", name);
			else len = snprintf(path, sizeof(path)-1, "%s/%s", name, ent->d_name);
            path[len] = 0;

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            compareFiles(path, level + 1,f);
        }
        //if file
        else{
        	len = snprintf(path, sizeof(path)-1, "%s", name);
        	if(strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.out")!=0 && strcmp(ent->d_name,"sha1.c")!=0 && strcmp(ent->d_name,"sha1.h")!=0 && strcmp(ent->d_name,"out.txt")!=0 && strcmp(ent->d_name,"gg.txt")!=0) {
        			strcpy(t,getLine(ent->d_name,f));

        			// aaaaaa.txt bug
        			// if "a" is in file

        			if(t!=NULL){
        				if(strcmp(t,getSha(path,ent->d_name))!=0){
        					char file[strlen(path)+strlen(ent->d_name)+1];

        					// file path
        					strcpy(file,path);
        					strcat(file,"/");
                            //strcat(file,"\"");
		        			strcat(file,ent->d_name);
                            //strcat(file,"\"");
		        			strcpy(t, file);

		        			strcat(t,"|");

		        			// file hash
        					strcat(t, getSha(path,ent->d_name));

        					strcat(t,"|");

        					// file size
        					char buffer[18];
                            printf("\n%s is changed\n",file);
        					sprintf(buffer, "%lu", getSize(file));
        					strcat(t,buffer);
							strcat(t,"|");

                            stat(file, &b);
							strftime(buffer, 100, "%m%d%Y-%H:%M:%S|", localtime(&b.st_ctime));
                            //printf("\n%s\n",buffer);
							strcat(t,buffer);

							strftime(buffer, 100, "%m%d%Y-%H:%M:%S|", localtime(&b.st_mtime));
							strcat(t,buffer);

							strftime(buffer, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_atime));
							strcat(t,buffer);

        					fprintf(file2, "%s\n",t);
        				}
        			}

				
        	}	
        }

    } while (ent = readdir(dir));
    
    closedir(dir);
}

int searchWord(char *fname, char *str) {
	FILE *f;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	
	if((f = fopen(fname, "r")) == NULL) return(-1);

	while(fgets(temp, 512, f) != NULL) {
		if((strstr(temp, str)) != NULL) {
			find_result++;
			return 1;
		}
		line_num++;
	}

	if(find_result == 0) return 0;
	
	//Close the file if still open.
	if(f) fclose(f);
	
   	return 0;
}

char* getLine(char *str,FILE *f) {
	char *temp=malloc(512);
	char fname[strlen(str)+2];
    char *buf;
	strcpy(fname,"\"");
	strcat(fname,str);
	strcat(fname,"\"");

	while(fgets(temp, 512, f) != NULL){
		if(strstr(temp,fname)!=NULL) return strtok(temp, "|");
	}
	return NULL;
}

off_t getSize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0) return st.st_size;

    return -1; 
}

void test(int err, const char* msg) {
    if (err) {
        fprintf(stderr, "%s: error %d\n", msg, err);
        exit(1);
    }
}

char* getSha(char* fileDirectory, char* fileName){

    char	buffer[20];
    char	*eureka=malloc(100);
    char	file[strlen(fileDirectory)+strlen(fileName)+1];
    int 	n,
    		i;

    static	unsigned char buf[20] = {0};
    SHA1Context sha;
    FILE 	*f;
    memset(eureka, 0, sizeof eureka);

    strcpy(file,fileDirectory);
    strcat(file,"/");
    strcat(file,fileName);
    f=fopen(file,"rb");
    int size=getSize(file);

	char string[size];

	if(!f) return;

	fseek(f, SEEK_SET, 0);


	if(size==0) strcpy(string,"");
	else while(fgets(buffer,20,f)!=NULL) strcat(string,buffer);

	
    test(SHA1Reset(&sha), "SHA1Reset");
    test(SHA1Input(&sha, string, size), "SHA1Input");
    test(SHA1Result(&sha, buf), "SHA1Result");

    int int_hash = 0;
    for (i=0; i<20; ++i){
        sprintf(buffer, "%02X", buf[i]);
        int_hash += buf[i];
        strcat(eureka,buffer);
    }

	return eureka;

}

void callInit(){
/*
file1=fopen("gg.txt","r");
char line[150];
char *buf;
while(fgets(line,150,file1)!=NULL){
    buf = strtok (line,"\"");
    buf = strtok (NULL,"\"");
    printf("\n%s",buf);
}

fclose(file1);
*/
    file1=fopen("gg.txt","w+");
    initFileList("./Galu",0,file1);
    fclose(file1);
}