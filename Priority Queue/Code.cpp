#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct tnode{
	char name[26];
	int age;
	char desc[101];
	int code;
	struct tnode *next;
} *head, *tail;

void insert(){
	struct tnode *temp = (struct tnode*) malloc(sizeof(struct tnode));
	
	bool valid = 0;
	
	while(!valid){
		printf("Input patient name[4-25]: ");
		scanf("%[^\n]", &temp->name);
		getchar();
		
		if(strlen(temp->name) < 4 || strlen(temp->name) > 25){
			continue;
		}		
		
		valid = 1;
	}
	
	valid = 0;
	
	while(!valid){
		printf("Input patient age[>= 0]: ");
		scanf("%d", &temp->age);
		getchar();
		
		if(temp->age < 0){
			continue;
		}
		
		valid = 1;
	}
	
	valid = 0;
	
	while(!valid){
		printf("Input description[>= 6 characters]: ");
		scanf("%[^\n]", &temp->desc);
		getchar();
		
		if(strlen(temp->desc) < 6){
			continue;
		}
		
		valid = 1;
	}
	
	valid = 0;
	
	while(!valid){
		char tempCode[11];
		
		printf("Input code[Red|Yellow|Green]: ");
		scanf("%[^\n]s", &tempCode);
		getchar();
		
		if(strcmp(tempCode, "Red") != 0 && strcmp(tempCode, "Yellow") != 0  && strcmp(tempCode, "Green") != 0){
			continue;
		}
		else if(strcmp(tempCode, "Red") == 0){
			temp->code = 3;
		}	
		else if(strcmp(tempCode, "Yellow") == 0){
			temp->code = 2;
		}
		else{
			temp->code = 1;
		}
		
		valid = 1;
	}
	
	temp->next = NULL;
	
	if(head == NULL){
		head = temp;
		tail = temp;
	}
	else{
		struct tnode *curr = head;
	
		if(temp->code > head->code){
			temp->next = head;
			head = temp;
		}			
		else if(tail->code >= temp->code){
			tail->next = temp;
			tail = temp;
			temp->next = NULL;
		}
		else{			
			while(curr->next->code >= temp->code){
				curr = curr->next;
			}
			
			if(temp->code < curr->next->code){
				curr = curr->next;
			}

			temp->next = curr->next;
			curr->next = temp;
		}
	}
	printf("Insert success !\n\n");
}

void view(){
	if(head == NULL){
		printf("There is no queue yet!\n\n");
	}
	else{
		struct tnode *curr = head;
		
		printf("Patient List: \n");
		printf("-----------------------------------------------------------------------------------------------------------------\n");
		printf("| No\t| %-40s| Age\t| %-40s| %-12s|\n", "Name", "Description", "Code");
		printf("-----------------------------------------------------------------------------------------------------------------\n");
		
		int i = 1;

		while(curr != NULL){
			char tempCode[7];
			if(curr->code == 3){
				strcpy(tempCode, "Red");
			} 
			else if(curr->code == 2)
			{
				strcpy(tempCode, "Yellow");
			} 
			else{
				strcpy(tempCode, "Green");
			}
			
			printf("| %d\t| %-40s| %d\t| %-40s| %-12s|\n", i, curr->name, curr->age, curr->desc, tempCode);
			
			curr = curr->next;
			i++;
		}
		printf("-----------------------------------------------------------------------------------------------------------------\n");
		printf("\n\n");
	}
}

void nextQueue(){
	if(head == NULL){
		printf("There is no queue yet!\n\n");
	}
	else{		
		if(head == NULL){
			printf("There is no queue yet!\n\n");
			return;
		}
		
		printf("The next patient is:\n");
		printf("Name\t\t: %s\n", head->name);
		printf("Age\t\t: %d\n", head->age);
		printf("Description\t: %s\n", head->desc);
		printf("Code\t\t: ");
		if(head->code == 3) printf("Red");
		else if(head->code == 2) printf("Yellow");
		else printf("Green");	
		
		struct tnode *temp = head;
		
		head = head->next;
		free(temp);
		
		printf("\n\n\n");
	}
}

int main(){	
	int option = 0;
	
	while(option != 4){
		printf("Bluejack Hospital\n");
		printf("=================\n");
		printf("1. Insert\n");
		printf("2. View\n");
		printf("3. Next Queue\n");
		printf("4. Exit\n");
		printf(">> ");
		
		scanf("%d", &option);
		getchar();
		
		if(option > 4 || option < 1){
			continue;
		}
		
		switch(option){
			case 1:
				insert();
				break;
			case 2:
				view();
				break;
			case 3:
				nextQueue();
				break;
		}	
	}
	
	return 0;	
}
