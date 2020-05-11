/** Author: Tom Hua
	slinkedlist.c
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

typedef struct slinkedlist* type_Node;
 
_Bool append(int d);
_Bool insert_d_infront_n(int v, int d);
void delete(int d); // delete the node having d
int reverseList(void);
int showlist(void);
int cleanupmem(void);

type_Node  createNode(void);

struct slinkedlist{
    int d;
    struct slinkedlist* pNext;
};

type_Node head = NULL;
type_Node current = NULL;   
static int nodecnt;

int main(void)
{
    int n = 0;
    _Bool ret = false;
    type_Node temp = NULL;
    
    ret = append(123);
    printf("%d node has: %d\n", nodecnt, current->d);
    ret = append(456);
    printf("%d node has: %d\n", nodecnt, current->d);
    insert_d_infront_n(0, 789);
    ret = append(789);
    printf("%d node has: %d\n", nodecnt, current->d);
    insert_d_infront_n(0, 789);    
    delete(456);

    printf("\n"); 
    printf("my singly linked list:\n");
    showlist();    
    printf("\n"); 

    if (reverseList() > 0)
    {
        printf("my reversed singly linked list:\n");
        showlist(); 
    }
    printf("\n"); 
     
    printf("clean up ...\n");        
    n = cleanupmem(); 
    if (n>0)
        printf("List of %d nodes removed. Memory released\n", n);  
                             
    printf("---------------------------------\n"); 
	return 0;
}

type_Node createNode(void)
{
    type_Node temp = (type_Node)(malloc(sizeof(struct slinkedlist)));
    if (temp == NULL) {
        perror("failed to allocate memory.\n");
        return NULL;
    }
    
    temp->pNext = NULL;
     
    return temp;
}

_Bool append(int d)
{
    type_Node newnode = NULL;
    newnode = createNode();
    if (newnode == NULL) {
        perror("failed to append a node.\n");
        return false;   
    }
    
    newnode->d = d;
    if (head == NULL) {
        head = newnode;
        current = head;
        head->pNext = NULL;
    }
    else {    
        current->pNext = newnode;
        current = newnode;
    }
    nodecnt++; 
    return true;            
}

_Bool insert_d_infront_n(int v, int d)
{   // insert v in front of the node having d
    type_Node temp = head;
    type_Node current = head;
    type_Node newnode = NULL;
    newnode = createNode();
    if (newnode == NULL) {
        perror("failed to append a node.\n");
        return false;   
    }
    while (temp)
    {
        if (d == temp->d) {
            current->pNext = newnode; // link the previous to the new node
            newnode->pNext = temp;    // link the new node to the next
            nodecnt++;
            printf("inserted a new node having %d\n", v);
            return true;
        }
        else {
            current = temp;     // save the current one as the previous one
            temp = temp->pNext; // advance the to next
        }
    }
    
    printf(" No insertion. Node having %d is not found.\n", d);
    return false;   // could not find the node having d
}

void delete(int d)
{
    type_Node temp = head;
    type_Node current = head;
    while (temp)
    {
        if (d == temp->d) {
            current->pNext = temp->pNext; // link the previous to the next
            free(temp);
            nodecnt--;
            printf("deleted node having %d\n", d);
            return;
        }
        else {
            current = temp;     // save the current one as the previous one
            temp = temp->pNext; // advance the to next
        }
    }

}

int cleanupmem(void)
{
    int n = 0;
    current = NULL;
    type_Node temp = head;

    while (temp)    // start from the head
    {
        current = temp->pNext; // advance the current to the next
        free(temp);     // free the head current one 
        temp = current;    // now the current node becomes the head
        n++;
    }
    return n;
}


int reverseList(void)
{
    type_Node temp = head;
    type_Node current = head;
    type_Node target = NULL;
    
    if(head == NULL)
        return -1;  // empty list
    if (head->pNext == NULL)
        return 0;   // signle node
        
    while(current)
    {
        temp = current; // 1. save the current
        current = current->pNext;   // 2. advance the current
        temp->pNext = target;   // 3. NULL in the first loop
        target = temp;  // 4. target is the head in the first loop
        // from the second loop on, all pointers traverse toward the end
    }
    head = target;
    
    return 1;
}


int showlist(void)
{
    int n = 0; 
    
    type_Node temp = head;
    if (temp == NULL)
        return 0;
    while (temp)
    {
        printf(" %d", temp->d);
        temp = temp->pNext;
        n++;
    }
    printf("\n"); 
    return n;
}

