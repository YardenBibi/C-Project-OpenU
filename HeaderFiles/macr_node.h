

typedef struct macr_node{
    char *macr_name;
    char *macr_code;
    int macr_row;
    struct macr_node *next;
} macr_node;