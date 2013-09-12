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
			if(strcmp(ent->d_name,"..")==1 && strcmp(ent->d_name,"a.c")==1 && strcmp(ent->d_name,"a.exe")==1 && strcmp(ent->d_name,"gg.txt")==1 && strcmp(ent->d_name,"out.txt")==1){

				// prints date of modification
				if (!stat(ent->d_name, &b)){
					strftime(t, 100, "%d/%m/%Y %H:%M:%S|||", localtime(&b.st_mtime));
					strcat(t,ent->d_name);
					printf("searching for string '%s' in gg.txt :",t);
					if(searchWord("gg.txt",t)==0) fprintf(f2, "%s\n",t);
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
	
/*	if(!f){
		printf("Creating file...\n");
		f=fopen("gg.txt","w+");
		printf("'gg.txt' created!\n\n");
	}*/
	
	
	if ((dir = opendir ("C:\\Users\\Raf\\Desktop\\NDSGthesis\\Code\\Raf")) != NULL) {
	
	  while ((ent = readdir (dir)) != NULL){

		if(strcmp(ent->d_name,"..")==1 && strcmp(ent->d_name,"a.c")==1 && strcmp(ent->d_name,"a.exe")==1 && strcmp(ent->d_name,"gg.txt")==1 && strcmp(ent->d_name,"out.txt")==1){
			
			// prints date of modification
			if (!stat(ent->d_name, &b)){
				strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime(&b.st_mtime));
				fprintf(f, "%s|||%s\n",t,ent->d_name);
			} else printf("Cannot display the time.\n");
			
		} //endif
	  } //endwhile

	  closedir (dir);
	}else perror ("");
	fclose(f);
}



int searchWord(char *fname, char *str) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	
	//gcc users
	if((fp = fopen(fname, "r")) == NULL) {
		return(-1);
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
			//printf("A match found on line: %d\n", line_num);
			//printf("\n%s\n", temp);
			find_result++;
			printf("1\n");
			return 1;
		}
		line_num++;
	}

	if(find_result == 0) {
		//printf("Sorry, couldn't find a match.\n");
		printf("0\n");
		return 0;
	}
	
	//Close the file if still open.
	if(fp) fclose(fp);
   	return 0;
}