#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct FileList{
	struct FileList *next;
	char hash[100];
	int size;
	char cDate[20];
	char mDate[20];
	char aDate[20];
}FileList;

FileList* GetList(){
	FILE *f;
	char str[100]="";
	
	FileList	*fl = (FileList*)malloc(sizeof(FileList)),
				*node,
				*head;
	
	f=fopen("Raf/out.txt","r");
	if(!f) printf("NO FILE");
	head=fl;
	fgets(str,100,f);
	if(strcmp(str,"")==0) return NULL;

	strcpy(head->hash,strtok(str, "|"));
	head->size=atoi(strtok(NULL, "|"));
	strcpy(head->cDate,strtok(NULL, "|"));
	strcpy(head->mDate,strtok(NULL, "|"));
	strcpy(head->aDate,strtok(NULL, "|"));
	
	while(fgets(str,100,f)!=NULL){
			node=(FileList*)malloc(sizeof(FileList));
			strcpy(node->hash,strtok(str, "|"));
			node->size=atoi(strtok(NULL, "|"));
			strcpy(node->cDate,strtok(NULL, "|"));
			strcpy(node->mDate,strtok(NULL, "|"));
			strcpy(node->aDate,strtok(NULL, "|"));
			head->next=node;
			head=head->next;
	}
	
	head->next=NULL;
	fclose(f);
	return fl;
}

void printList(){
	FileList *fl=GetList();
	if(fl!=NULL){
		while(1){
			printf("%s|",fl->hash);
			printf("%d|",fl->size);
			printf("%s|",fl->cDate);
			printf("%s|",fl->mDate);
			printf("%s",fl->aDate);
			if(fl->next==NULL) break;
			fl=fl->next;
			printf("\n");
		}
	}else{
		printf("NO OUTPUT");
	}
}
