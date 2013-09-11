#include <time.h>
#include <stdio.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include<dirent.h> // for dir algo
void initFileList();
void compareFiles();

void main() {
	int i;
	printf("1 to initialize txt file\n2 to compare\n:");
	scanf("%d",&i);
	if(i==1) initFileList();
	else compareFiles();
	
}

void compareFiles(){
	// algo
	FILE *f;
	f=fopen("gg.txt","r+");
	if(!f){
		printf("There is no file named 'gg.txt'!\nExiting...");
		return;
	}
	fprintf(f,"ASDF");
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
	
	
	if ((dir = opendir ("C:")) != NULL) {
	
	  while ((ent = readdir (dir)) != NULL){

		if(strcmp(ent->d_name,"..")==1){
			
			// prints files in the directory
			//printf("%s ",ent->d_name);

			
			// prints date of modification
			if (!stat(ent->d_name, &b)){
				strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime(&b.st_mtime));
				fprintf(f, "%s|||%s\n",t,ent->d_name);
			} else printf("Cannot display the time.\n");
			
		} //endif
	  } //endwhile

	  closedir (dir);
	}else perror ("");
}