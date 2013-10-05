//IGNORE SMALL FILES

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

void masterServer(FILE *f1, FILE *f2) {

	char line[300], fullline[300];
	char *token, **ptr;
	char *splithash[30];
	int i, j, a;
	long num;
	long entries = 0;
	long hashcatalogentries = 0;
	
	//check length of new input
	while (fgets (line, sizeof(line), f1) != NULL) {
		entries++;
	}
	
	//check length of hashcatalog
	while (fgets (line, sizeof(line), f2) != NULL) {
		hashcatalogentries++;
	}
	
	rewind(f1);
	rewind(f2);
	bloom_init(&bloom, hashcatalogentries + entries, 0.01);
	
	//add hashcatalog entries to bloom filter
	while (fgets (line, sizeof(line), f2) != NULL) {
	
		i = 0;
		splithash[i] = strtok(line, "|");
		while (splithash[i] != NULL)
			splithash[++i] = strtok(NULL, "|");
		
		bloom_add(&bloom, splithash[1], strlen(splithash[1]));
	}
	
	//read list of updated files
	while (fgets (line, sizeof(line), f1) != NULL) {
		
		i = 0;
		fullline = line;
		strcpy(fullline, line);
		splithash[i] = strtok(line, "|");
		while (splithash[i] != NULL)
			splithash[++i] = strtok(NULL, "|");
		
		//Size Filter
		num = atoi(splithash[2]);
		if (num < 8000) {
			continue;
		}
	
		//Bloom Filter		
		if (bloom_add(&bloom, splithash[1], strlen(splithash[1])) == 0) {
			fprintf(f2, "%s", fullline);
		} else {
			printf("hello %s", fullline);
		}
	
	}
	
}

main () {

	FILE *f1, *f2;
	f1 = fopen ("out.txt", "r");
	f2 = fopen ("hashcatalog.txt", "r+");
	
	//Function to read file with hash functions and ignore small files
	masterServer(f1, f2);
	
}