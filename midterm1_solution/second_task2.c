#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>

typedef struct listelement
{
    struct listelement *prev,*next;
    int number;
}listelement;

listelement *head = NULL;

void new_element(int a);
void print_list();
void remove_element(int d);
void delete_list();

int main()

{
    remove_element(3);
    new_element(5);
    new_element(5);
    new_element(5);
    new_element(5);
    print_list();
    remove_element(2);
    printf("after you delete ----------------\n");
    print_list();
    delete_list();
    printf("after you delete whole thing----)");
    print_list();

   
}

void delete_list()
{
    while (head != NULL)
    {
        free(head);
        head = head->next;
    }
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

void remove_element(int d)
{
    listelement *current = head;
    if (head == NULL)
    {
        printf("Nothing to remove!\n");
        return;
    }
    while (current != NULL)
    {
        if (current->number == d)
        {
            listelement *prev, *next;
            prev = current -> prev;
            next = current -> next;
            if (prev) prev->next = next;
            if (next) next->prev = prev;
            free(current);
            if (current == head) head = next;


        }
        current = current->next;
    }

}

void new_element(int a)
{
    listelement *current = head;
    if (head == NULL)
    {
        current = (listelement*)malloc(sizeof(listelement));
        current -> prev = NULL;
        current -> next = NULL;
        current->number = a;
        head = current;
    }

    listelement *newElement;
    newElement = (listelement*)malloc(sizeof(listelement));
    newElement->number = a;
    newElement->next = head;
    newElement->prev  = NULL;
    head->prev = newElement;
    head = newElement;


}