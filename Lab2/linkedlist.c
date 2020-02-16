//
//  main.c
//  Lab2_listElement
//
//  Created by Lam Tran on 1/11/20.
//  Copyright © 2020 Lam Tran. All rights reserved.
//

#include <stdio.h>

struct listelement* push_String();
struct listelement* delete_item();
void end_program();

void print_list();

//Use this structure:

struct listelement
{
    struct listelement *next,*prev;
    char text[1000];
};
//and this global list head:
struct listelement *head = NULL;



int main() {
    // insert code here...
    printf("Please enter an input!\n");
    printf("Select:\n1.Push String \n2.Print List \n3.Delete Item \n4. End Program\nINPUT = ");
    
    
//    struct listelement *ob = NULL;
    
    int input;
    int itemnumber;
    char text[1000];
    scanf("%d", &input);
    
    while ((input == 1) || (input == 2) || (input == 3))
    {
        if (input == 1)
        {
            printf("enter a text: \n");
            scanf("%s", text);
            head = push_String(text, head);
            printf("Done push string \n");
            

        }
        if (input == 2)
        {
            print_list(head);

        }
        if (input == 3)
        {
            printf("Enter the item number you want to delete\n");
            scanf("%i", &itemnumber);
            head = delete_item(itemnumber, head);
        }
        
        printf("\n######################################\nSelect:\n1.Push String \n2.Print List \n3.Delete Item \n4. End Program\nINPUT = ");
        scanf("%d", &input);

        
    }
    
    end_program(head);
    
    
    return 0;
}



void print_list(struct listelement *object)
{
    int number = 1;
    if (object == NULL)
    {
        printf("Nothing to print");
    }
    while (object != NULL)
    {
        printf("%i %s\n", number, object->text);
        object = object->next;
        number++;
    }
}

void end_program(struct listelement *current)
{
    struct listelement *head = current;
    head = NULL;
}
struct listelement* delete_item(int number, struct listelement *current)
{
    struct listelement *head = current;
    struct listelement *previous = current;
    int count = 1;
    
    if (current == NULL)
    {
        printf("Nothing to delete\n");
        return head;
    }
    
   
    while (current != NULL)
    {

        if (count == number)
        {


            if ((current->next != NULL) && number == 1)
            {
                printf("deleting first element");
                previous = current -> prev;
                current = current -> next;
                current->prev = previous;
                return current;
            }
            else if ((current->next == NULL) && number == 1)
            {
                printf("deleting only element");
                return NULL;
            }
            else if (current->next != NULL)
            {
                printf("\nDeleting not last element");
                current = current->next;
                previous->next = current;
                current->prev = previous;
                return head;
            }
            else if ((current->next) == NULL )
            {
                printf("\nDeleting last element");
                previous->next = current->next;
                current = current ->next;
                return head;
            }
            
            
        }
        previous = current;
        current = current->next;
        count = count + 1;
        
    }
    printf("Out of index\n");
    return head;
}

struct listelement* push_String(char word[1000], struct listelement *current)
{
    struct listelement *head = current;

    struct listelement *previous = NULL;


    if (current == NULL)
    {
        previous = current;
        current = (struct listelement *) malloc(sizeof(struct listelement));
        head = current;
        strcpy(current->text, word);
        current->next = NULL;
        current->prev = previous;

        current = current->next;
        return head;
    }
    
    while (current->next != NULL)
    {
        previous = current;
        current = current->next;
        
    }
    
    current->next = (struct listelement *) malloc(sizeof(struct listelement *));
    
    current = current->next;
    
    //current->text[1000] = word[1000];
    strcpy(current->text, word);
    current->prev = previous;

    current->next = NULL;
    
    
    return head;
}

