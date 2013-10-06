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

void masterServer(FILE *f1, FILE *f2, FILE *f3) {

	char line[300], fullline[300], cacheline[300];
	char *splithash[10];
	int i, j, a;
	long num, entries = 0, hashcatalogentries = 0;
	//int nobloom = 0;
	
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
		/*while (fgets (cacheline, 300, f3) != NULL) {
			cacheline[strlen(cacheline) - 1] = '\0';
			if (splithash[1] == cacheline) {
				printf("IT EXISTS SA CACHEEEEEEEEEEEE\n");
				rewind(f2);
				//do something about the folder madness here
				//folderCache (splithash[0], f2, f3);
				nobloom = 1;
			} else
				printf("WALA HUHUHU\n");
		}*/
		
		//Nobloom = 1 if meron sa cache, no need to continue to bloom
		/*if (nobloom) {
			nobloom = 0;
			continue;
		}*/
			
		//Bloom Filter
		if (bloom_add(&bloom, splithash[1], strlen(splithash[1])) == 0) {
			fprintf(f2, "%s", fullline);
		}
	}
	
}

//Adding folders and files in File Cache
void folderCache (char *path, FILE *f2, FILE *f3) {
	
	//Using path, get all strings in hashcatalog that start with the same path
	
	//strncmp
	
	
}

main () {

	FILE *f1, *f2, *f3;
	f1 = fopen ("out.txt", "r");
	f2 = fopen ("hashcatalog.txt", "r+");
	f3 = fopen ("cache.txt", "r+");
	
	//Function to read file with hash functions and ignore small files
	masterServer(f1, f2, f3);
	
}