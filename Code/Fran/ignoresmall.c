//IGNORE SMALL FILES

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "bloom.h"

struct bloom bloom;

void masterServer(FILE *f1) {

	char line[300];
	char *token, **ptr;
	char *splithash[30];
	int i, j, a;
	long num;
	long entries = 0;
	
	while (fgets (line, sizeof(line), f1) != NULL) {
		entries++;
	}
	
	bloom_init(&bloom, entries, 0.01);
	rewind(f1);
	
	while (fgets (line, sizeof(line), f1) != NULL) {
		
		i = 0;
		splithash[i] = strtok(line, " ");
		while (splithash[i] != NULL)
			splithash[++i] = strtok( NULL, " " );
		
		//Size Filter
		num = atoi(splithash[2]);
		if (num < 8000) {
			printf("FAIL! %ld\n", num);
			continue;
		} 
		
		//Bloom Filter
		bloom_add(&bloom, splithash[0], strlen(splithash[0]));

	}
	
	if (bloom_check(&bloom, "him", 3)) {
		printf("It may be there!\n");
	}	

}

main () {

	FILE *f1;
	f1 = fopen ("test.txt", "r");
	
	//Function to read file with hash functions and ignore small files
	masterServer(f1);
	
}