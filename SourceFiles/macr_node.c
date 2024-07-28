#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/common.h"
#include "../HeaderFiles/handle_errors.h"
#include "../HeaderFiles/assembly_utils.h"
#include "../HeaderFiles/handle_macr.h"

macr_node* create_node(char *name,char *code, int row) {
    macr_node *new_node;
    new_node = manual_malloc(sizeof(macr_node));
    if (new_node == NULL) {
        general_errors("Failed to allocate memory for new node");
        return NULL;
    }
    new_node->macr_name= name;
    new_node->macr_code = code;
    new_node->macr_row = row;
    new_node->next = NULL;
    
    return new_node;
}

void free_nodes_list(macr_node *head) {
    macr_node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->macr_name);
        free(temp->macr_code);
        free(temp);
        
    }
}

void print_list(macr_node *head) {
    while (head != NULL) {
        printf("Name: %s\nCode: %s\nRow: %d\n", head->macr_name, head->macr_code, head->macr_row);
        head = head->next;
    }
}

int add_node(char * file_name,macr_node **head, char *name, char *code, int row) {
    macr_node *new_node,*temp;
    macr_node *current = *head;
    char error_msg[INITIAL_BUFFER_SIZE];

    while (current != NULL) {
        if (strcmp(current->macr_name, name) == 0) {
            if (strcmp(current->macr_code, code) == 0) {
                return 0; 
            } else {
                sprintf(error_msg,"A node with the name '%s' already exists but with a different code.\n", name);
                input_errors(file_name,row,error_msg);
                return -1;
            }
        }
        current = current->next;
    }

    new_node = create_node(name, code, row);
    if (new_node == NULL) {
        return -1;
    }

    if (*head == NULL) {
        *head = new_node;
        return 1;
    }

    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = new_node;
    
    return 1; 
}