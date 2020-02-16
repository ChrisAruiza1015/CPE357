/* SEJAL KARGAL CPE 357 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>


typedef struct listelement
{
    struct listelement *prev, *next;
    int number;
} listelement;

listelement *listhead = 0;
struct listelement *cur = 0;


void new_element(int element)
{
    listelement *p = (listelement*)malloc(sizeof(listelement));
    if (listhead == 0)
    {
        listhead = p;
        p -> number = element;
        p -> prev = p -> next = 0;
        return;
    }
    else
    {
        p -> next = listhead;
        listhead -> prev = p;
        listhead = p;
        p -> number = element;
        return;
    }
}

void printlist()
{
    if (listhead == 0)
    {
        printf("empty list\n");
        return;
    }
    listelement *p = listhead;
    for(;p!=0;p=p->next)
    {
        printf("Data: %d\n", p -> number);
        printf("Prev: %x\n", (unsigned int)p -> prev);
        printf("Next: %x\n", (unsigned int)p -> next);
    
    }
}

void remove_element(int d)
{
    listelement *p = listhead;
    for(;p!=0;)
    {
        if (p -> number == d)
        {
            listelement *A, *B, *C;
            A = p ->prev;
            B = p;
            C = p -> next;
            if(A) A -> next = C;
            if(C) C -> prev = A;
            if(listhead == p)listhead = C;
            free(p);
            p = listhead;
        }
        else
        {
            p = p -> next;
        }
        
    }
}

void delete_list()
{
    struct listelement* cur = listhead;
    struct listelement* next;
    while (cur != 0)
    {
    next = cur -> next;
    free(cur);
    cur = next;
    }
listhead = 0;
}

int main()
{
    new_element(5);
    new_element(5);
    new_element(5);
    new_element(5);
    printlist();
    printf("**************\n");
    remove_element(5);
  //  printlist();
    // new_element(6);
    printlist();
    return 0;
}






