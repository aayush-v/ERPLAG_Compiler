// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include<string.h>
#include<stdlib.h>
#include "hashtable.h"

/*All hashtables that use the following functions will be of the form of an array of structure variables.
 * The structure can be defined with any sort of data members, but the first element in the
 * structure must a character pointer that stores the key value of the key value pair.
 */

/*The following is the hash function used. This function is supposed to be fairly effective.*/
unsigned long int hashFunc(char *string, int size)
{
	unsigned long int hash=7;
	for(int i=0;i<size;i++){
		hash=(hash<<5)+hash+string[i];
	}
	return hash;
}

/* As mentioned, this set of functions works with a wide range of possible structures.
 * The following functions sets the key field (the initial character pointer) to all the members of the array as NULL.
 * In essence, this initializes the hashtable as empty (checking the key value for NULLness is used to see if that
 * array slot is empty).
 */
hashTable* initHashTable()
{	
	hashTable* h1 = (hashTable*)malloc(sizeof(hashTable));
	h1->scope[0] = 0;
	h1->scope[1] = 0;
	h1->size = 0;
	h1->ioSize = 0;
	h1->locSize = 0;
	h1->locIoSize = 0;
	h1->first = NULL;
	h1->current = NULL;
	//h1->initSize = 0;
	h1->nextTemp = 1;
	//h1->currentUse = 0;
	h1->parentEntry = NULL;
	for(int i=0;i<TABLESIZE;i++){
		h1->Table[i] = NULL;
	}
}

/* This function enters *value in the hash table with key *key. There is no restriction on what type value can be.
 * The copy function must appropriately copy information stored in value to the table structure.
 */
int insert(hashTable *h1, char *key, astNode* type)
{
	int keylen=strlen(key);
	int ind=hashFunc(key, keylen)%TABLESIZE, i=0;
	while((i<TABLESIZE) && h1->Table[(ind+i*i)%TABLESIZE]!=NULL){
		if(strcmp(h1->Table[(ind+i*i)%TABLESIZE]->id,key)==0){
			return -2;
		}
		i++;
	}
	if(i==TABLESIZE){
		return -1;
	}
	else{
		h1->Table[(ind+i*i)%TABLESIZE] = (hashEntry*)malloc(sizeof(hashEntry));
		hashEntry* e1 = h1->Table[(ind+i*i)%TABLESIZE];
		strcpy(e1->id,key);
		e1->use = 0;
		e1->extra = NULL;
		e1->isStatic = 1;
		e1->forBlock = 0;
		e1->whileBlock = 0;
		e1->whileUse = 0;
		e1->whileHas = 0;
		e1->io = 0;
		e1->check = NULL;
		e1->type = type;
		e1->offset = 0;
		e1->locOffset = 0;
		e1->width = 0;
		e1->partOf = h1;
		e1->next = NULL;
		e1->derive = NULL;

		return (ind+i*i)%TABLESIZE;
	}
}

/*This function frees the memory allocated to the key field of the structure element that forms each element in the hash table array*/
void freeHashTable(hashTable* h1)
{
	for(int i=0;i<TABLESIZE;i++){
		if(h1->Table[i]){
			free(h1->Table[i]);
		}
	}
	free(h1);
}

/*This function returns the index in which the key value pair corresponding to the given key *key is stored.*/
int retrieve(hashTable *h1, char *key)
{
	int ind=hashFunc(key, strlen(key))%TABLESIZE, i=0;
	while((i<TABLESIZE) && (h1->Table[(ind+i*i)%TABLESIZE]!=NULL) && (strcmp(key,h1->Table[(ind+i*i)%TABLESIZE]->id))){
		i++;
	}
	if(i==TABLESIZE){
		return -2;
		//Couldnt find such a value, that's all yeah?
	}
	//Will this be a problem?
	hashEntry* e1 = h1->Table[(ind+i*i)%TABLESIZE];
	if(e1){
		return (ind+i*i)%TABLESIZE;
	}
	else{
		return -2;
	}
	//If the return is NULL, we know there is an error.
}
