//
//  main.c
//  Lab2_listElement
//
//  Created by Lam Tran on 1/11/20.
//  Copyright © 2020 Lam Tran. All rights reserved.
//

#include <stdio.h>

void new_element();
void remove_element(int d);

void delete_list();
void print_list();

//Use this structure:

typedef struct listelement
{
    struct listelement *next,*prev;
    int number;
}listelement;
//and this global list head:
listelement *head = NULL;



int main() {
    
    remove_element(3);
    new_element();
    new_element();
    new_element();
    new_element();
    print_list();
    remove_element(5);
    printf("after you delete ----------------\n");
    print_list();
   

}

void remove_element(int d)
{
    
    listelement *currentNode = head;
    if (currentNode == NULL)
    {
        printf("Nothing to remove \n");
        return;
    }
    listelement *nextE;

    while (currentNode != NULL)
    {
        
        if (currentNode -> number == d)
        {
            listelement *prev, *current, *next;
            prev = currentNode -> prev;
            next = currentNode -> next;
            if (prev) prev->next = next;
            if (next) next->prev = prev;
            free(currentNode);
            if (head == currentNode) head = next;
        }
       
        currentNode = currentNode->next;
    }
    // head = currentNode;
}

void print_list ()
{
    listelement *current = head;
    while (current!= NULL)
    {
        printf("Current Node Address: %x\n",current);
        printf("Number = %d\n", current->number);
        printf("Next = %x\n", current->next);
        printf("Prev = %x\n", current->prev);

        current = current->next;
    }
}

void new_element()
{
    listelement *current;
    listelement *previous = NULL;
    printf("What is you number? \n");
    int n;
    scanf("%d", &n);
    if (head == NULL)
    {
        current = (struct listelement*)malloc(sizeof(listelement));
        head = current;
        head->number = n;
        head->prev = NULL;
        head->next = NULL;

        return;
    }

    listelement *newElement = (struct listelement*)malloc(sizeof(listelement));
    newElement->number = n;
    newElement->next = head;
    newElement->prev = NULL;
    head->prev = newElement;
    head = newElement;
    

    

}