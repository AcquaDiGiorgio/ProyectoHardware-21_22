#include <stddef.h>
#include <stdlib.h>
#include "cola.h"

struct Node *crearLista(uint64_t data){

}

struct Node *add(struct Node *last, uint64_t data)
{
  
	while (last == NULL){
		
	}
	last -> data = data;
		struct Node *newNode =
        (struct Node *)malloc(sizeof(struct Node));
		last -> next = newNode;
		return last;
  // Creating a node dynamically.
  struct Node *temp =
        (struct Node *)malloc(sizeof(struct Node));
   
  // Assigning the data.
  temp -> data = data;
 
  // Adjusting the links.
  temp -> next = last -> next;
  last -> next = temp;
  last = temp;
   
  return last;
}

uint64_t pop(struct Node *first)
{
	uint64_t val = first -> data;
	
	if (first -> next == NULL)
		first = NULL;	
	else
		first = first -> next;
	
	return val;	
}

