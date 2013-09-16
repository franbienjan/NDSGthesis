#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct FileList{
	struct FileList *next;
	char fileName[100];
}FileList;

FileList* GetList(){
	FILE *f;
	char str[100]="";
	
	FileList	*fl = (FileList*)malloc(sizeof(FileList)),
				*node,
				*head;
	
	f=fopen("../Code/Raf/out.txt","r");
	
	head=fl;
	fgets(str,100,f);
	strcpy(head->fileName,str);
	
	while(fgets(str,100,f)!=NULL){
			node=(FileList*)malloc(sizeof(FileList));
			strcpy(node->fileName,str);
			head->next=node;
			head=head->next;
	}
	
	fclose(f);
	return fl;
}

void main(){
	FileList*fl=GetList();
}