typedef struct macr_node{
    char *macr_name;
    char *macr_code;
    int macr_row;
    struct macr_node *next;
} macr_node;

macr_node* create_node(char *name,char *code, int row);

void free_nodes_list(macr_node *head);

void print_list(macr_node *head);

int add_node(char * file_name, macr_node **head,char *name, char *code, int row);