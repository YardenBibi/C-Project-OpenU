typedef struct macr_node{
    char *macr_name;
    char *macr_code;
    int macr_row;
    struct macr_node *next;
} macr_node;

/**
 * This function creates a new node for a linked list representing macros.
 * It allocates memory for the node and initializes it with the provided macro 
 * name, macro code, and the line number where the macro is defined. 
 * @param name Pointer to a null-terminated string representing the name of the macro.
 * @param code Pointer to a null-terminated string representing the code associated with the macro.
 * @param row Integer representing the line number in the source file where the macro is defined.
 * @return A pointer to the newly created `macr_node`. If memory allocation fails, 
 *         NULL is returned and an error message is printed using `general_errors`.
 */
macr_node* create_node(char *name,char *code, int row);

/**
 * This function frees all the nodes in a linked list of macros.
 * It traverses the list, freeing the memory allocated for each node.
 * @param head Pointer to the head of the linked list of `macr_node` structures. 
 *             If the list is empty (i.e., `head` is NULL), the function does nothing.
 */
void free_nodes_list(macr_node *head);

/**
 * This function traverses and prints the contents of a linked list of macros.
 * @param head Pointer to the head of the linked list of `macr_node` structures. 
 *             If the list is empty (i.e., `head` is NULL), the function does nothing.
 */
void print_list(macr_node *head);

/**
 * This function adds a new macro node to a linked list of macros. It first checks if a node 
 * with the same macro name already exists in the list. If so, the function does nothing and returns 0.
 * If a node with the same name but a different code exists, an error is reported.
 * If no such node exists, the new node is created and added to the end of the list.
 * @param file_name Pointer to the null-terminated string representing the name of the file 
 *                  where the macro was defined. This is used for error reporting.
 * @param head Pointer to the pointer to the head of the linked list of `macr_node` structures. 
 * @param name Pointer to the null-terminated string representing the name of the macro.
 * @param code Pointer to the null-terminated string representing the code of the macro.
 * @param row The line number in the file where the macro was defined.
 *
 * @return 1 if the node is successfully added, 0 if a node with the same name and code 
 *         already exists, and -1 if an error occurs.
 */
int add_node(char * file_name, macr_node **head,char *name, char *code, int row);