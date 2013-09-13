//IGNORE SMALL FILES

#include <stdio.h>
#include <ctype.h>
#include <string.h>

void sizeFilter(FILE *f1) {

	char line[300];
	char *token;
	char *splithash[30];
	int i, j;
	
	while (fgets (line, sizeof(line), f1) != NULL) {
		
		i = 0;
		splithash[i] = strtok(line, " ");
		while (splithash[i] != NULL)
			splithash[++i] = strtok( NULL, " " );
		
		for (j = 0; j < i; j++) {
			printf("%d '%s'\n", j, splithash[j]);
		}
	}

}

main () {

	FILE *f1;
	f1 = fopen ("test.txt", "r");
	
	//Function to read file with hash functions and ignore small files
	sizeFilter(f1);
	
}
