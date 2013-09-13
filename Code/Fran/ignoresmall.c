//IGNORE SMALL FILES

#include <stdio.h>
#include <dirent.h> 
#include <sys/stat.h>

main () {

	DIR *dir;
	FILE * pFile;
	long lSize;
	
	struct dirent *ent;
	struct stat b;
	
	if ((dir = opendir ("C:\\Users\\Viernes Family\\Desktop")) != NULL) {
	
	  while ((ent = readdir (dir)) != NULL){

		if(strcmp(ent->d_name,"..")==1){
			
			pFile = fopen ( ent->d_name , "r" );
	
			if (pFile==NULL) {
				//printf("File error %s\n", ent->d_name);
				continue;
				//fputs ("File error",stderr); exit (1);
			}
			
			//obtain file size
			fseek (pFile , 0 , SEEK_END);
			lSize = ftell (pFile);
			rewind (pFile);
			
			if (lSize >= 8000)
				printf("%s: %d bytes\n", ent->d_name, lSize);
			
		}
	  }

	  closedir (dir);
	} else perror ("");
	
}