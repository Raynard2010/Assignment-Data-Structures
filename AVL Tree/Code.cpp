#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tnode{
	char title[26];
	char type[10];
	int stock;
	struct tnode *left;
	struct tnode *right;
	int height;
};

struct tnode* create_node(char title[], char type[], int stock){
	struct tnode *node = (struct tnode*) malloc(sizeof(struct tnode));
	strcpy(node->title, title);
	strcpy(node->type, type);
	node->stock = stock;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}

int height(struct tnode *N){
	if(N == NULL) return 0;
	return N->height;
}

int getBalance(struct tnode *N){
	if(N == NULL) return 0;
	return height(N->left) - height(N->right);
}

int max(int a, int b){
	if(a > b) return a;
	else return b;
}

struct tnode *minValueNode(struct tnode *node){
	struct tnode *curr = node;
	
	while(curr->left != NULL) curr = curr->left;
	
	return curr;
}

struct tnode *leftRotate(struct tnode *x){
	struct tnode *y = x->right;
	struct tnode *T2 = y->left;
	
	y->left = x;
	x->right = T2;
	
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;
	
	return y;
}

struct tnode *rightRotate(struct tnode *y){
	struct tnode *x = y->left;
	struct tnode *T2 = x->right;
	
	x->right = y;
	y->left = T2;
	
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;
	
	return x;
}

struct tnode *insert_node(struct tnode *root, char title[], char type[], int stock){
	if(root == NULL) return create_node(title, type, stock);
	
	if(strcmp(title, root->title) < 0) root->left = insert_node(root->left, title, type, stock);
	else if(strcmp(title, root->title) > 0) root->right = insert_node(root->right, title, type, stock);
	else return root;

	root->height = 1 + max(height(root->left), height(root->right));

	int balance = getBalance(root);
	
	if(balance > 1 && strcmp(title, root->left->title) < 0) return rightRotate(root);
		
	if(balance < -1 && strcmp(title, root->right->title) > 0) return leftRotate(root);

	if(balance > 1 && strcmp(title, root->left->title) > 0){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	if(balance < -1 && strcmp(title, root->right->title) < 0){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

void print_inorder(struct tnode *root){
	if(root == NULL) return;
	
	print_inorder(root->left);
	printf("| %-26s| %-10s | %-10d |\n", root->title, root->type, root->stock);
	printf("-------------------------------------------------------\n");
	print_inorder(root->right);
}

struct tnode* search_node(struct tnode *root, char x[]){
	if(root == NULL || strcmp(root->title, x) == 0) return root;
	else if(strcmp(x, root->title) < 0) return search_node(root->left, x);
	else return search_node(root->right, x);
}

struct tnode* delete_node(struct tnode *root, char x[]){
	if(root == NULL) return root;
	else if(strcmp(x, root->title) < 0) root->left = delete_node(root->left, x);
	else if(strcmp(x, root->title) > 0) root->right = delete_node(root->right, x);
	else{
		if(root->left == NULL || root->right == NULL){
			struct tnode *temp = root->left ? root->left : root->right;
			
			if(temp == NULL){
				temp = root;
				root = NULL;
			}
			else *root = *temp;
			
			free(temp);
		}
		else{
			struct tnode *temp = minValueNode(root->right);
			
			strcpy(root->title, temp->title);
			strcpy(root->type, temp->type);
			root->stock = temp->stock;
			
			root->right = delete_node(root->right, temp->title);
		}
	}
	
	if(root == NULL) return root;
	
	root->height = 1 + max(height(root->left), height(root->right));
	
	int balance = getBalance(root);
	
	if(balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
	
	if(balance > 1 && getBalance(root->left) < 0){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	if(balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
	
	if(balance < -1 && getBalance(root->right) > 0){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	
	return root;
}

void insertGame(struct tnode **root){
	char newTitle[26];
	char newType[10];
	int newStock;
	
	bool valid = 0;
	
	while(!valid){
		printf("Input game title[5-25][unique]: ");
		scanf("%[^\n]", &newTitle);
		getchar();
		
		if(strlen(newTitle) < 5 || strlen(newTitle) > 25) continue;

		struct tnode* node = search_node(*root, newTitle);
		if(node != NULL) continue;
		
		valid = 1;
	}
	
	valid = 0;
	
	while(!valid){
		printf("Input game type[Action|RPG|Adventure|Card Game]: ");
		scanf("%[^\n]", &newType);
		getchar();
		
		if(strlen(newTitle) < 5 || strlen(newTitle) > 25) continue;
		
		if(strcmp(newType, "Action") != 0 && strcmp(newType, "RPG") != 0 && strcmp(newType, "Adventure") != 0 && strcmp(newType, "Card Game") != 0) continue;
		
		valid = 1;
	}
	
	valid = 0;
	
	while(!valid){
		printf("Input game stock[>= 1]: ");
		scanf("%d", &newStock);
		getchar();
		
		if(newStock < 1) continue;
		
		valid = 1;
	}
	
	*root = insert_node(*root, newTitle, newType, newStock);
	
	printf("Insert success !\n\n");
}

void viewGame(struct tnode *root){
	if(root == NULL){
		printf("Warehouse is empty !\n\n");
		return;
	}
	
	printf("-------------------------------------------------------\n");
	printf("| %-26s| %-10s | Game Stock |\n", "Game Title", "Game Genre");
	printf("-------------------------------------------------------\n");
	
	print_inorder(root);
	
	printf("\n\n");
}

void updateStock(struct tnode **root){
	if(root == NULL){
		printf("Warehouse is empty !\n\n");
		return;
	}
	
	char gameToUpdate[26];
	printf("Input game title: ");
	scanf("%[^\n]", &gameToUpdate);
	getchar();
	
	struct tnode* node = search_node(*root, gameToUpdate);
	if(node == NULL){
		printf("Data not found !\n\n");
		return;
	};
	
	printf("Current stock: %d\n\n", node->stock);
	
	char updateType[7];
	char lowerUpdateType[7];
	
	bool valid = 0;
	
	while(!valid){
		printf("Input update type[add|remove][case insensitive]: ");
		scanf("%[^\n]", &updateType);
		getchar();
		
		strcpy(lowerUpdateType, strlwr(updateType));
		
		if(strcmp(lowerUpdateType, "add") != 0 && strcmp(lowerUpdateType, "remove") != 0) continue;

		valid = 1;
	}
	
	if(strcmp(lowerUpdateType, "add") == 0){
		valid = 0;
		
		int addStock;
		
		while(!valid){
			printf("Input stock to add[>= 1]: ");
			scanf("%d", &addStock);
			getchar();
			
			if(addStock < 1) continue;
			
			valid = 1;
		}
		
		node->stock = node->stock + addStock;
		printf("Data updated successfully !\n\n");
	}
	else{
		valid = 0;
		
		int removeStock;
		
		while(!valid){
			printf("Input stock to remove[1-%d]: ", node->stock);
			scanf("%d", &removeStock);
			getchar();
			
			if(removeStock < 1 || removeStock > node->stock) continue;
			
			valid = 1;
		}
		
		node->stock = node->stock - removeStock;
		
		if(node->stock == 0){
			struct tnode *curr = *root;
			
			printf("Data updated successfully !\n");
			printf("%s is removed from the warehouse !\n\n", node->title);
			
			if(strcmp(curr->title, node->title) == 0 && curr->left == NULL && curr->right == NULL){
				free(*root);
				*root = NULL;
			}
			else delete_node(*root, node->title);
		}
		else{
			printf("Data updated successfully !\n\n");			
		}
	}
}

int main(){
	struct tnode *root = NULL;

	int nchoice = 0;
	
	while(nchoice != 4){
		printf("Bluejack GShop\n");
		printf("==============\n");
		printf("1. Insert Game\n");
		printf("2. View Game\n");
		printf("3. Update Stock\n");
		printf("4. Exit\n");
		printf(">> ");
		
		scanf("%d", &nchoice);
		getchar();		
		
		switch(nchoice){
			case 1:
				insertGame(&root);
				break;
			case 2:
				viewGame(root);
				break;
			case 3:
				updateStock(&root);
				break;
		}
	}
	
	return 0;
}
