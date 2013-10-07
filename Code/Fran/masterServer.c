//**********************************************************************
//* TASKS TO COMPLETE:
//* [X] Fix Bloom Filter Data Structure
//* [X] Compare and filter out duplicate files
//* [X] Ignore and filter out small files (<8000)
//* [ ] Read paper on SIFL and File Locality
//*     >> [ ] Compare file to folder (in extreme binning-ish technique)
//*     >> [ ] Stream Informed File Layout technique
//*		>> [ ] Cache files
//**********************************************************************

//**********************************************************************
//* NOTES:
//*	[X] Hash lang ang ilagay Bloom Filter, pero store yung whole
//* [ ] Produce a return text file for LCD Stage of the game
//* [ ] 
//*
//**********************************************************************

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "bloom.h"

struct bloom bloom;

//Adding folders and files in File Cache
//Using path, get all strings in hashcatalog that start with the same path
void folderCache (char *path, char *hash, FILE *f2, FILE *f3) {

	char linus[300], *splithash[30], *splithash2[30];
	int i = 0;
	
	rewind(f2);
	fclose(f3);
	f3 = fopen("cache.txt", "w+");
	
	splithash[i] = strtok(path, "/");
	while (splithash[i] != NULL)
		splithash[++i] = strtok(NULL, "/");

	splithash[i-1] = '\0';
	fprintf(f3, "%s\n", hash);
	
	while (fgets (linus, 300, f2) != NULL) {
		
		if (strncmp (*splithash, linus, strlen(*splithash)) == 0) {
			
			i = 0;
			splithash2[i] = strtok(linus, "|");
			while (splithash2[i] != NULL)
				splithash2[++i] = strtok(NULL, "|");
	
			fprintf(f3, "%s\n", splithash2[1]);
		}
	}
	
	rewind(f3);
	
}

void masterServer(FILE *f1, FILE *f2, FILE *f3) {

	char line[300], fullline[300], cacheline[300];
	char *splithash[10];
	int i, j, a;
	long num, entries = 0, hashcatalogentries = 0;
	int nobloom = 0;
	
	//check length of new input
	while (fgets (line, 300, f1) != NULL) {
		entries++;
	}
	
	//check length of hashcatalog
	while (fgets (line, 300, f2) != NULL) {
		hashcatalogentries++;
	}
	
	rewind(f1);
	rewind(f2);
	bloom_init(&bloom, hashcatalogentries + entries, 0.01);
	
	//add hashcatalog entries to bloom filter
	while (fgets (line, 300, f2) != NULL) {
	
		i = 0;
		splithash[i] = strtok(line, "|");
		while (splithash[i] != NULL)
			splithash[++i] = strtok(NULL, "|");
		
		bloom_add(&bloom, splithash[1], strlen(splithash[1]));
	}
	
	//read list of updated files
	while (fgets (line, 300, f1) != NULL) {
		
		i = 0;
		fullline = line;
		splithash[i] = strtok(line, "|");
		while (splithash[i] != NULL)
			splithash[++i] = strtok(NULL, "|");
		
		//Size Filter
		num = atoi(splithash[2]);
		if (num < 8000) {
			continue;
		}
			
		//Cache Check; NOTE: Hash lang ang nakastore sa Cache
		while (fgets (cacheline, 300, f3) != NULL) {
			cacheline[strlen(cacheline) - 1] = '\0';
			
			printf("CHECK IN!! %s || %s\n", cacheline, splithash[1]);
			if (strcmp(splithash[1], cacheline) == 0) {
				printf("IT EXISTS SA CACHEEEEEEEEEEEE\n");
				nobloom = 1;				
			} else
				printf("WALA HUHUHU\n");
				
		}
		
		//Nobloom = 1 if meron sa cache, no need to continue to bloom
		if (nobloom) {
			nobloom = 0;
			continue;
		}
			
		//Bloom Filter
		if (bloom_add(&bloom, splithash[1], strlen(splithash[1])) == 0) {
			folderCache (splithash[0], splithash[1], f2, f3);
			fprintf(f2, "%s", fullline);
		}
	}
	
}

main () {

	FILE *f1, *f2, *f3;
	f1 = fopen ("out.txt", "r");
	f2 = fopen ("hashcatalog.txt", "r+");
	f3 = fopen ("cache.txt", "r+");
	
	//Function to read file with hash functions and ignore small files
	masterServer(f1, f2, f3);
	
}