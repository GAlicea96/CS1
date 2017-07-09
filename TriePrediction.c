/*Guillermo Alicea
12/05/15
Gu843115, g3626806
Assignment 6*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"

// Helper function called by printTrie() (Credit: Prof. Szumlanski)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if(root == NULL)
		return;

	if(root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for(i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Prof. Szumlanski)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if(useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

//create each individual node in the Trie
TrieNode *createNode()
{
	int i;
	TrieNode *node = malloc(sizeof(TrieNode));
	
	node->count = 0;
	
	for(i = 0; i < 26; i++)
		node->children[i] = NULL;

	node->subtrie = NULL;
	
	return node;
}

//Called from buildTrie to insert each word into the Trie, taking into account all possibilities of input, such
//as apostrohpes in words or upper case letters. Does not needlessly call createNode, as it checks whether
//or not this is necessary.
void mainTrieBuilder(TrieNode *temp, int length, char *buffer, char *stBuffer, int check)
{
	int i;

	
	for(i = 0; i < length; i++)
	{
		if(buffer[i] == '!' || buffer[i] == '.' || buffer[i] == '?')
		{
				check = 0;
				break;
		}
		else if(buffer[i] >= 65 && buffer[i] <= 90)
			buffer[i] = tolower(buffer[i]);
	
		else if((buffer[i] < 97 || buffer[i] > 122) || buffer[i] == 32)
			continue;
		
		if(temp->children[buffer[i] - 'a'] == NULL)
			temp = temp->children[buffer[i] - 'a'] = createNode();
	
		else if(temp->count >= 1 && i + length == length)
			break;	
	
		else
				temp = temp->children[buffer[i] - 'a'];		
	}
	
	temp->count++;
	if(check)
	{
		if(temp->subtrie == NULL)
			temp->subtrie = createNode();
	
		mainTrieBuilder(temp->subtrie, strlen(stBuffer), stBuffer, "", 0);
	}
}

//Builds the trie based on the file holding the body of text ("corpus--.txt")
TrieNode *buildTrie(char *filename)
{
	
	FILE *ptr = fopen(filename, "r");
	TrieNode *root = createNode(), *temp = root;
	int length, i = -1, j, numSentences = 0;
	char buffer[MAX_CHARACTERS_PER_WORD], *sentence[MAX_CHARACTERS_PER_WORD], c[MAX_CHARACTERS_PER_WORD];

	while(fscanf(ptr, "%s", c) == 1)
		numSentences++;

	for(i = 0; i < numSentences; i++)
		sentence[i] = malloc(numSentences);

	rewind(ptr);	

	i = -1;
	while(fscanf(ptr, "%s", sentence[++i]) == 1)
		;

	for(j = 0; j < i; j++)
		mainTrieBuilder(temp, strlen(sentence[j]), sentence[j], sentence[j+1], 1);

	fclose(ptr);
	
	for(i = 0; i < numSentences; i++)
		free(sentence[i]);

	return root;
}

//Finds the most frequently occuring word in the subtrie of a node passed into
//the function.
int findWord(TrieNode *root, char *buffer, int k, int *maxP, char *winner)
{
	int i;

	if(root == NULL)
		return *maxP;

	if(root->count > *maxP)
	{
		*maxP = root->count;
		strcpy(winner, buffer);
	}

	buffer[k + 1] = '\0';

	for(i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;
		
		*maxP = findWord(root->children[i], buffer, k + 1, maxP, winner);
	}

	buffer[k] = '\0';
	
	return *maxP;
}

//Returns the address of the subtrie of last node in a word's pathway. 
TrieNode *goToWordSubtrie(TrieNode *root, char *string)
{
	int i, length = strlen(string);
	
	
	for(i = 0; i < length; i++)
	{
		if(string[i] >= 65 && string[i] <= 90)
			string[i] = tolower(string[i]);
		root = root->children[string[i] - 'a'];
	}
	
	return root->subtrie;
}

//Checks whether a word is in the trie or not.
int inTrieOrNah(TrieNode *root, char *string)
{
	int i, length = strlen(string), nah = 0;
	
	
	for(i = 0; i < length; i++)
	{
		if(string[i] >= 65 && string[i] <= 90)
			string[i] = tolower(string[i]);
	
		if(root->children[string[i] - 'a'] == NULL)
		{
			nah = 1;
			break;
		}	
	
		else	
			root = root->children[string[i] - 'a'];
	}
	
	if(nah)
		return 0;
	else
		return 1;
}

//Called by destroyTrie as a helper function.
TrieNode *freeNode(TrieNode *root)
{
	int i;
	
	if(root == NULL)
		return root;

	for(i = 0; i < 26; i++)
	{

		if(root->children[i] != NULL)
		{
			if(root->children[i]->subtrie != NULL)
				freeNode(root->children[i]->subtrie);
			
			freeNode(root->children[i]);
		}
	}
		
	free(root);
	root = NULL;
	
	return root;
}

//Frees and sets to null all allocated memory within a trie.
//Recursively frees from the bottom up, starting with subtries then children.
//Works with the help of freeNode.
TrieNode *destroyTrie(TrieNode *root)
{
	int i;
	
	for(i = 0; i < 26; i++)
	{
		if(root->children[i] != NULL)
			freeNode(root->children[i]->subtrie);
	
		freeNode(root->children[i]);
	}

	free(root);
	root = NULL;
	
	return root;
}

//Challenging but easier than assignment 4
double difficultyRating(void)
{
	return 3.5;
}

//Spread out over the course of a few days
double hoursSpent(void)
{
	return 8;
}

int main(int argc, char **argv)
{
	//not checking whether argc == 3 because it is guaranteed in the input.
	TrieNode *root = buildTrie(argv[1]), *temp = root;
	char winner[MAX_CHARACTERS_PER_WORD], buffer[MAX_CHARACTERS_PER_WORD], 
	command[MAX_CHARACTERS_PER_WORD], *hold;
	FILE *ptr;
	int n, i, max = 0, *maxP = &max;

	ptr = fopen(argv[2], "r");
	
	//Based on "input--.txt", this while loop runs, determining what actions to perform, until the end
	//of the file is reached. 
	while(fscanf(ptr, "%s", command) == 1)
	{
		temp = root;
	
		//case of '!'
		if(command[0] == '!')
			printTrie(root, 0);
		
		//case of '@'
		else if(command[0] == '@')
		{
			fscanf(ptr, "%s", winner);
			printf("%s", winner);
		
			if(inTrieOrNah(root, winner))
			{	
				fscanf(ptr, "%d", &n);
				printf(" ");
			
				for(i = 0; i < n; i++)
				{
					temp = goToWordSubtrie(root, winner);
				
					if(temp != NULL)
					{
						max = 0;
						findWord(temp, buffer, 0, maxP, winner);
					}
				
					else
					{
						break;	
					}
					
						//determine proper formatting based on termination, whether it occurs
						//earlier than expected or not
						printf("%s%c", winner, ((i != n - 1)  && goToWordSubtrie(root, winner) != NULL)
						? ' ' : '\n');
				}
			}
		
			else
			{
				printf("\n");
				fscanf(ptr, "%s", command);
			}
		}
		
		//case of <string>
		else
		{
			hold = malloc(sizeof(command));
			strcpy(hold, command);
			printf("%s\n", hold);
		
			if(inTrieOrNah(root, command))
				{
					temp = goToWordSubtrie(root, command);
				
					if(temp == NULL)
						printf("(EMPTY)\n");
				
					else	
						printTrie(goToWordSubtrie(root, command), 1);								
				}
		
			else
				printf("(INVALID STRING)\n");
		
			free(hold);
		}
	}

	//root is freed and set to NULL after destroyTrie is called
	root = destroyTrie(root);
	fclose(ptr);
	
	return 0;
}
