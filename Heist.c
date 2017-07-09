/*Guillermo Alicea
11/13/15
Recitation 6
g3626806, gu843115*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//avl tree struct
typedef struct AVL
{
	int data;
	struct AVL *leftChild, *rightChild;
} AVL;
//used in balancing function
int max(int a, int b)
{
	return (a > b) ? a : b;
}
//calculates the height of the root passed to it (recursive)
int height(AVL *root)
{
	if(root == NULL)
		return 0;

   return max(height(root->leftChild), height(root->rightChild)) + 1;
} 
//checks if a tree is balanced using the above 2 functions (recursive)
int isBalanced(AVL *root)
{	
	int heightDiff;
	
	if(root == NULL) 
		return 1;

	heightDiff = height(root->leftChild) - height(root->rightChild);
	
	if(abs(heightDiff) > 1) 
		return 0;
	else
		return isBalanced(root->leftChild) && isBalanced(root->rightChild);
}
//function to create a node in tree
AVL *createNode(int data)
{
	AVL *root = malloc(sizeof(AVL));
	root->data = data;
	root->leftChild = NULL;
	root->rightChild = NULL;

	return root;
}
//used to insert nodes into tree passed in by input
AVL *insertNode(AVL *root, int data)
{
	if (root == NULL)
		return createNode(data);

	// insert into left subtree
	else if (data < root->data)
		root->leftChild = insertNode(root->leftChild, data);

	// insert into right subtree
	else if (data > root->data)
		root->rightChild = insertNode(root->rightChild, data);

	return root;
}
//resets the tree after each tree is completely filled based on its inputs
AVL *resetTree(AVL *root)
{
	if(root == NULL)
		return root;
	
	resetTree(root->rightChild);
	resetTree(root->leftChild);
	
	free(root);
	root = NULL;
	return root;

}

int main()
{
	int i, j, k, NOT = 0, numTrees, insert;
	AVL *root = NULL;
	
	//loop through all input
	scanf("%d", &numTrees);
	for(i = 0; i < numTrees; i++)
	{
		scanf("%d", &j);
		for(k = 0; k < j; k++)
		{
			scanf("%d", &insert);
			if(root == NULL)
				root = createNode(insert);
			else
			insertNode(root, insert);
			if(!isBalanced(root))
			{
				NOT = 1;
			}	
		}
		//NOT is used to determine if a tree needs to be removed, while keeping
		//the scanning organized based on the above code
		if(NOT)
		{
			printf("Tree #%d: REMOVE\n", i+1);
			NOT = 0;
		}
		else
			printf("Tree #%d: KEEP\n", i+1);
		
		root = resetTree(root);
	}
}
