#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} Node;

Node* createNode(int d){
    Node *newNode = malloc(sizeof(Node));
    newNode->data = d;
    newNode->next = NULL;
    return newNode;
}

int main() {
    Node *head = createNode(10);
    Node *second = createNode(20);
    Node *third = createNode(30);
    return 0;
}