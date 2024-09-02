typedef struct table {
    long long init;
    long long end;
} BCP;

typedef struct node {
    int val;
    struct node *bef;
    struct node *next;
} Node;

typedef struct q {
    Node *head;
    Node *end;
} Queue;

int is_empty(Queue *q); //1 if empty, 0 otherwise
int enqueue(Queue *q, Node *val); //1 if okay, 0 if error
int dequeue(Queue *q); //val of element