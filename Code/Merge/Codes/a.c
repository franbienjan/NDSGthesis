#include <time.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
//#include <sys/types.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "sha1.c"

// searches the string 'str' in file 'fname'
int searchWord(char *fname, char *str) ;

// gets the line in 'initFiles.txt' with the 'str' string
char* getLine(char *str,FILE *f);

// prints out the changed files and puts in out.txt
void compareFiles(const char *name, int level,FILE *f);

// initializes the file list in initFiles.txt
void initFileList(const char *name, int level,FILE *f);

// returns the size of the file in bytes
off_t getSize(const char *filename);

// test function for getSha()
void test(int err, const char* msg);

// gets "sha-1"
char* getSha(char* fileDirectory, char* fileName);

long int convertUTF8ToUnicode(FILE *fileHandler, FILE *fOut, int c);
// initializes initFiles.txt
void callInit(char *str);
/*
 *	initFiles.txt - information of files
 *  out.txt - list of files to be deduplicated
 *
 */

FILE *file1;
FILE *file2;
typedef struct FolderList{
    char *fname;
    struct FolderList *next;
}FolderList;

void raf() {
	int i=0,  
        folderNum=0,
        j=0,
        last=0;
    DIR *dir;
    FolderList *folder, *head, *node, *lastNode;
    struct dirent *ent;
    folder=(FolderList*)malloc(sizeof(FolderList));

    lastNode=NULL;
    if (!(dir = opendir("."))) return;
    if (!(ent = readdir(dir))) return;
    do{
        if (ent->d_type == DT_DIR){
            if(strcmp(ent->d_name,"..")!=0 && strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"Codes")!=0 && strcmp(ent->d_name,"Outputs")!=0 && strcmp(ent->d_name,"chunkstempofolder")!=0){
                if(folderNum==0){
                    folder->fname=ent->d_name;
                    head=folder;
                }else{
                    node=(FolderList*)malloc(sizeof(FolderList));
                    node->fname=ent->d_name;
                    head->next=node;
                    head=head->next;
                    head->next=NULL;
                }
                folderNum++;
            }
        }
    }while (ent = readdir(dir));
    do{
        head=folder;
        // display options
        for(i=1 ; i<=(folderNum+2) ; i++){
            if(i<folderNum+1){
                printf("\n[%d] Initialize '%s'",i, head->fname);
                head=head->next;
            }else{
            	if(i==folderNum+1){
	            	if(lastNode==NULL) printf("\n[%d] Compare [No folder initialized yet]",i);
	            	else printf("\n[%d] Compare [%s]",i,lastNode->fname);
            	}
                else printf("\n[%d] %s\n:",i,"Exit");
            }
        }
        scanf("%d",&i);
        head=folder;
        if(i<folderNum+1 && i>0){
            last=i;
            for(j=1 ; j<i ; j++) head=head->next;
            char temp[strlen(head->fname)+4];
            strcpy(temp,"./");
            strcat(temp,head->fname);
            file1=fopen("initFiles.txt","w+");
            initFileList(temp,0,file1);
            fclose(file1);
            lastNode=head;
        }else if(i==folderNum+1){
            for(j=1 ; j<last ; j++) head=head->next;
            char temp[strlen(head->fname)+4];
            strcpy(temp,"./Outputs/out.");
            strcat(temp,head->fname);
            file1=fopen("initFiles.txt","r");
            file2=fopen(temp,"w+");
            
            strcpy(temp,"./");
            strcat(temp,head->fname);
            compareFiles(temp,0,file1);
            fclose(file1);
            fclose(file2);
            callInit(temp);
        }else if(i==0){
            printf("ASPMOEFNEOWNFWEOFNEWOIFNOWEIFNWOE:IFNEW:FNWE:FNEWOIF");
            FILE *ff;
            ff=fopen("hashcatalog.txt","w");
            fclose(ff);
            ff=fopen("cache.txt","w");
            fprintf(ff,"onwoief\n");
            fclose(ff);

        }else break;
    }while(1);
/*

	do{
		printf("1 to initialize txt file\n2 to compare\n3 to exit\n:");
		scanf("%d",&i);
		if(i==1){
			file1=fopen("initFiles.txt","w+");
			initFileList("./Galu",0,file1);
			fclose(file1);
		}else if(i==2){
			file1=fopen("initFiles.txt","r");
			file2=fopen("out.txt","w+");
			compareFiles("./Galu",0,file1);
			fclose(file1);
			fclose(file2);
			callInit();
		}else if(i==3){
			break;
		}
	}while(1);
    */
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
        	printf("\nscanning: %s",ent->d_name);
        	//if(strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.out")!=0 && strcmp(ent->d_name,"sha1.c")!=0 && strcmp(ent->d_name,"sha1.h")!=0 && strcmp(ent->d_name,"out.txt")!=0 && strcmp(ent->d_name,"initFiles.txt")!=0) {        		
    		char file[strlen(path)+strlen(ent->d_name)+1];
    		strcpy(file,path);
    		strcat(file,"/");
    		strcat(file,ent->d_name);

			stat(file, &b);

            // for jpeg exceptions
            
            if(strstr(strrchr(ent->d_name, '.') , ".jpg")!=NULL){
                FILE *tempF, *jpgF;
                char tempStr[strlen(path)+strlen(ent->d_name)+1];
                char tempStr2[strlen(ent->d_name)+4];

                strcpy(tempStr,path);
                strcat(tempStr,"/");
                strcat(tempStr,ent->d_name);
                //printf("\n%s",tempStr);

                jpgF=fopen(tempStr,"r");
                strcat(tempStr,".txt");
                tempF=fopen(tempStr,"w");

                convertUTF8ToUnicode(jpgF, tempF,0);
                while(!feof(jpgF)) convertUTF8ToUnicode(jpgF, tempF,1);
                strcpy(tempStr2,ent->d_name);
                strcat(tempStr2,".txt");
                fprintf(f, "%s|", getSha(path,tempStr2));
                if(remove(tempStr)==0) printf("\nDeleted:%s",tempStr);

            }else fprintf(f, "%s|", getSha(path,ent->d_name));

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

        	//}	
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
        	//if(strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.out")!=0 && strcmp(ent->d_name,"sha1.c")!=0 && strcmp(ent->d_name,"sha1.h")!=0 && strcmp(ent->d_name,"out.txt")!=0 && strcmp(ent->d_name,"initFiles.txt")!=0) {
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
        	//}	
        }

    } while (ent = readdir(dir));
    
    closedir(dir);
}
/*
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
*/
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
    if (stat(filename, &st)==0) return st.st_size;
    return 0;
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


	strcpy(string,"");
	while(fgets(buffer,20,f)!=NULL) strcat(string,buffer);
    if(size==0)strcpy(string,"");
	
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

void callInit(char *str){
/*
file1=fopen("initFiles.txt","r");
char line[150];
char *buf;
while(fgets(line,150,file1)!=NULL){
    buf = strtok (line,"\"");
    buf = strtok (NULL,"\"");
    printf("\n%s",buf);
}

fclose(file1);
*/
    file1=fopen("initFiles.txt","w+");
    initFileList(str,0,file1);
    fclose(file1);
}

long int convertUTF8ToUnicode(FILE *f, FILE *fOut, int c){

    unsigned char charBuffer[1], helpBuffer[4];
    long int unicodeValue;

    fread(&charBuffer, sizeof(char), 1, f);
    if (charBuffer[0] < 0x80) unicodeValue = charBuffer[0];
    else if (charBuffer[0] < 0xc0) return -1;
    else if (charBuffer[0] < 0xe0){
        fread (&helpBuffer, sizeof(char), 2, f);
        unicodeValue = ( (charBuffer[0]&0x1f ) << 6 ) + (helpBuffer[0]&0x3f );
    }else if (charBuffer[0] < 0xf8) {
        fread (&helpBuffer, sizeof(char), 3, f);
        unicodeValue = ( (charBuffer[0]&0x0f ) << 12 ) + ((helpBuffer[0]&0x3f ) << 6 ) + (helpBuffer[1]&0x3f );
    }else if (charBuffer[0] < 0xfc) {
        fread (&helpBuffer, sizeof(char), 4, f);
        unicodeValue = ((charBuffer[0] & 0x07) << 18) + ((helpBuffer[1]&0x3f ) << 12 ) + ((helpBuffer[1]&0x3f ) << 6 ) +     (helpBuffer[2]&0x3f );
    }

    if(c==1)fprintf(fOut,"%lu",unicodeValue);
}