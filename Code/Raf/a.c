#include <time.h>
#include <stdio.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include<dirent.h> // for dir algo

#include<string.h>
#include<stdlib.h>
int searchWord(char *fname, char *str) ;
void compareFiles();
void initFileList();
off_t getSize(const char *filename);

/*
 *	gg.txt - information of files
 *  out.txt - list of files to be duplicated
 *
 */

void main() {
	int i;
	FILE *fp;
	do{
		printf("1 to initialize txt file\n2 to compare\n3 to exit\n:");
		scanf("%d",&i);
		if(i==1) initFileList();
		else if(i==2) compareFiles();	
		else if(i==3){
//fp=fopen("gg.txt","r");
//		readFile(fp);
//Search_in_File("gg.txt","out.txt");
			break;
		}
	}while(1);
}

void compareFiles(){
	DIR *dir;
	char t[ 100 ] = "";
	struct dirent *ent;
	struct stat b;
	FILE 	*f,
			*f2;
	f=fopen("gg.txt","r");
	f2=fopen("out.txt","w+");
	
	if ((dir = opendir ("C:\\Users\\Raf\\Desktop\\NDSGthesis\\Code\\Raf")) != NULL) {
		while ((ent = readdir (dir)) != NULL){
		
			// excludes unnecessary scans
			if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0 && strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.exe")!=0 && strcmp(ent->d_name,"gg.txt")!=0 && strcmp(ent->d_name,"out.txt")!=0){

				// prints date of modification
				if (!stat(ent->d_name, &b)){
					
					if(searchWord("gg.txt",ent->d_name)!=0){
					
						char buffer[50];
						strcpy(t,ent->d_name);
						strcat(t,"|");
						strcat(t,itoa(getSize(ent->d_name),buffer,10));
						strcat(t,"|");
						strftime(buffer, 100, "%m%d%Y-%H:%M:%S|", localtime(&b.st_ctime));
						strcat(t,buffer);
						strftime(buffer, 100, "%m%d%Y-%H:%M:%S|", localtime(&b.st_mtime));
						strcat(t,buffer);
						strftime(buffer, 100, "%m%d%Y-%H:%M:%S", localtime(&b.st_atime));
						strcat(t,buffer);
						
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


void initFileList(){
	DIR *dir;
	char t[ 100 ] = "";
	struct dirent *ent;
	struct stat b;
	FILE *f;
	f=fopen("gg.txt","w+");
	
	if ((dir = opendir ("C:\\Users\\Raf\\Desktop\\NDSGthesis\\Code\\Raf")) != NULL) {
		while ((ent = readdir (dir)) != NULL){
	  
			// excludes unnecessary scans
			if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0 && strcmp(ent->d_name,"a.c")!=0 && strcmp(ent->d_name,"a.exe")!=0 && strcmp(ent->d_name,"gg.txt")!=0 && strcmp(ent->d_name,"out.txt")!=0){
				
				// prints file info
				if (!stat(ent->d_name, &b)){
					// get file name
					fprintf(f, "%s|",ent->d_name);
					printf("%s|",ent->d_name);
					
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
					fprintf(f, "%s\n",t);
					printf("%s\n",t);
					
				} else printf("Cannot display the time.\n");
				
			} //endif
	  } //endwhile
	  closedir (dir);
	}else perror ("");
	fclose(f);
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
