#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int nbook = 0;

struct UniqueID{
	char bookID[7];
	char uISBN[14];
	char fauthor;
	char ftitle;
};

struct tnode{
	struct UniqueID uniqueID;
	char title[51];
	char author[26];
	char ISBN[14];
	int page;
	struct tnode *next;
};

struct tnode *head[1008] = {NULL};

void insert(){
	struct tnode *temp = (struct tnode*) malloc(sizeof(struct tnode));
	
	bool valid = 0;

	while(!valid){
		printf("Input book title[5-50][unique]: ");
		scanf("%[^\n]", &temp->title);
		getchar();
		
		bool titleExists = 0;
		
		if(strlen(temp->title) < 5 || strlen(temp->title) > 50){
			continue;
		}
		
		for(int i = 0; i < 1008; i++){
			struct tnode *curr = head[i];
			
			if(curr == NULL) continue;
			
			while(curr != NULL){
				if(strcmp(temp->title, curr->title) == 0){
					printf("The book title already exists !\n");
					titleExists = 1;
					break;
				}
				curr = curr->next;
			}
			
			if(titleExists) break;
		}
		if(titleExists) continue;

		valid = 1;
	}
	
	valid = 0;
	
	while(!valid){
		printf("Input author name[3-25][Mr. |Mrs. ]: ");
		scanf("%[^\n]", &temp->author);
		getchar();
		
		if((temp->author[0] != 'M' || temp->author[1] != 'r' || temp->author[2] != '.' || temp->author[3] != ' ') && (temp->author[0] != 'M' || temp->author[1] != 'r' || temp->author[2] != 's' || temp->author[3] != '.' || temp->author[4] != ' ')){
			continue;
		}		
		
		if(strlen(temp->author) < 3 || strlen(temp->author) > 25){
			continue;
		}
		
		valid = 1;
	}
	
	valid = 0;

	while(!valid){
		printf("Input ISBN[10-13][numeric]: ");
		scanf("%[^\n]", &temp->ISBN);
		getchar();
		
		bool notnumber = 0;
		
		if(strlen(temp->ISBN) < 10 || strlen(temp->ISBN) > 13){
			continue;
		}

		for(int i = 0; i < strlen(temp->ISBN); i++){
			if(temp->ISBN[i] < '0' || temp->ISBN[i] > '9'){
				notnumber = 1;
				break;
			}
		}
		
		if(notnumber) continue;		
		
		valid = 1;
	}
	
	valid = 0;
	
	while(!valid){
		printf("Input page number[>= 16 ]: ");
		scanf("%d", &temp->page);
		getchar();
		
		if(temp->page < 16){
			continue;
		}		
		
		valid = 1;
	}
	
	nbook++;
	
	int tempnbook = nbook;
	int bookIDidx = 5;
	
	strcpy(temp->uniqueID.bookID, "B00000");
	
	while(tempnbook > 0){
		temp->uniqueID.bookID[bookIDidx] = (tempnbook % 10) + 48;
		bookIDidx--;
		tempnbook = tempnbook / 10;
	}
	
	strcpy(temp->uniqueID.uISBN, temp->ISBN);
	
	if(temp->author[0] == 'M' && temp->author[1] == 'r' && temp->author[2] == '.'){
		temp->uniqueID.fauthor = temp->author[4];
	}
	else{
		temp->uniqueID.fauthor = temp->author[5];
	}
	
	temp->uniqueID.ftitle = temp->title[0];
	
	if(temp->uniqueID.fauthor >= 'a' && temp->uniqueID.fauthor <= 'z') temp->uniqueID.fauthor = temp->uniqueID.fauthor - 32;
	if(temp->uniqueID.ftitle >= 'a' && temp->uniqueID.ftitle <= 'z') temp->uniqueID.ftitle = temp->uniqueID.ftitle - 32;
	
	int hashkey = 45 + 45 + temp->uniqueID.fauthor + temp->uniqueID.ftitle;
	
	for(int i = 0; i < 6; i++){
		hashkey = hashkey + temp->uniqueID.bookID[i];
	}
	for(int i = 0; i < strlen(temp->ISBN); i++){
		hashkey = hashkey + temp->ISBN[i];
	}
	
	hashkey = hashkey % 1007;
	
	temp->next = NULL;
	
	if(head[hashkey] == NULL){
		head[hashkey] = temp;
	}
	else{
		struct tnode *curr = head[hashkey];
		
		while(curr->next != NULL) curr = curr->next;
		
		curr->next = temp;
	}
	
	printf("\nInsert success !\n\n");
}

void view(){
	bool nobook = 1;
	bool printhead = 1;
	
	for(int i = 0; i < 1008; i++){
		struct tnode *curr = head[i];
			
		if(curr == NULL) continue;
		
		if(printhead){
			printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("| %-23s | %-50s | %-25s | %-13s | Page Number |\n", "Book ID", "Book Title", "Book Author", "ISBN");
			printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
		}
		
		printhead = 0;
		nobook = 0;
					
		while(curr != NULL){
			char tempString[24] = {};
			strcpy(tempString, curr->uniqueID.bookID);
			strcat(tempString, "-");
			strcat(tempString, curr->ISBN);
			strcat(tempString, "-");
			
			int j = 0;
			
			while(tempString[j] != NULL) j++;
			tempString[j] = curr->uniqueID.fauthor;
			j++;
			tempString[j] = curr->uniqueID.ftitle;
			
			printf("| %-23s | %-50s | %-25s | %-13s | %-11d |\n", tempString, curr->title, curr->author, curr->ISBN, curr->page);
			printf("------------------------------------------------------------------------------------------------------------------------------------------\n");

			curr = curr->next;
		}
		
	}
	
	if(nobook) printf("There is no book(s) !\n");	
	
	printf("\n");
}

void deleteBook(){
	char bookToDelete[24];
	printf("Input book id to delete: ");
	scanf("%s", &bookToDelete);
	getchar();
	
	struct tnode *temp;
	char tempBookID[24];
	int key;
	int depth = 0;
	
	bool bookNotFound = 1;
	
	for(int i = 0; i < 1008; i++){
		struct tnode *curr = head[i];
		
		if(curr == NULL) continue;
		
		int k = 0;
		
		while(curr != NULL){
			char tempString[24] = {};
			strcpy(tempString, curr->uniqueID.bookID);
			strcat(tempString, "-");
			strcat(tempString, curr->ISBN);
			strcat(tempString, "-");
			
			int j = 0;
			
			while(tempString[j] != NULL) j++;
			tempString[j] = curr->uniqueID.fauthor;
			j++;
			tempString[j] = curr->uniqueID.ftitle;
			
			if(strcmp(bookToDelete, tempString) == 0){
				bookNotFound = 0;
				temp = curr;
				strcpy(tempBookID, tempString);
				key = i;
				depth = k;
				break;
			}
			
			curr = curr->next;
			k++;
		}
			
		if(!bookNotFound) break;
	}
	
	if(bookNotFound){
		printf("Book not found !\n\n");
		return;
	}
	
	printf("\n");
	printf("Book Id: %s\n", tempBookID);
	printf("Book Title: %s\n", temp->title);
	printf("Book Author: %s\n", temp->author);
	printf("Book ISBN: %s\n", temp->ISBN);
	printf("Page Number: %d\n", temp->page);
	
	char tempShortBookID[7];
	
	strncpy(tempShortBookID, tempBookID, 6);
	
	char confirm;
	
	do{
		printf("Are you sure [y|n]? ");
		scanf("%c", &confirm);
		getchar();
	}while(confirm != 'y' && confirm != 'n');
	
	if(confirm == 'n'){
		printf("\nDelete cancelled !\n\n");
		return;
	}
	else{
		if(depth == 0){
			struct tnode *del = head[key];
			head[key] = head[key]->next;
			free(del);
		}
		else{
			struct tnode *curr = head[key];
			
			while(strcmp(tempShortBookID, curr->next->uniqueID.bookID) != 0) curr = curr->next;
			
			if(curr->next->next == NULL){
				struct tnode *del = curr->next;
				
				curr->next = NULL;
				free(del);
			}
			else{
				struct tnode *del = curr->next;
				
				curr->next = curr->next->next;
				free(del);
			}
		}
	}
	
	printf("\nDelete success !\n\n");
}

int main(){	
	int option = 0;
	
	while(option != 4){
		printf("Bluejack Library\n");
		printf("=================\n");
		printf("1. View Book\n");
		printf("2. Insert Book\n");
		printf("3. Remove Book\n");
		printf("4. Exit\n");
		printf(">> ");
		
		scanf("%d", &option);
		getchar();
		
		if(option > 4 || option < 1){
			continue;
		}
		
		switch(option){
			case 1:
				view();
				break;
			case 2:
				insert();
				break;
			case 3:
				deleteBook();
				break;
		}	
	}
	
	return 0;	
}
