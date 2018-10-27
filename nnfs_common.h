
typedef struct node {
    char *name; // name of the directory of file
    node *parent; // pointer to parent
    node *nextChild; // array of children
    int isDir; // 1 if directory
    long long offSet;
    long long size;
    int deleted; // if this      
}node;