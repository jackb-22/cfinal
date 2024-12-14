#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct book{
    int id;
    char *title;
    char *author;
    char *category;
    char *status;
}Book;

typedef struct Node {
    Book book;
    struct Node *next;
}Node;

void deallocate(Node** root) {
   Node* curr = *root;
    while (curr != NULL) {
        Node* aux = curr;
        curr = curr->next;
        free(aux);
    }
    *root = NULL;
}

void insert_end(Node** root, Book value) {
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(1);
    }
    new_node->next = NULL;
    new_node->book = value;
    
    if (*root == NULL) {
        *root = new_node;
        return;
    }
    
    Node* curr = *root;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
}

void LoadBooks(FILE *f, Node*root){
    int id;
    char *title, *author, *category, *status;
    title = (char*)malloc(sizeof(char)*50);
    author = (char*)malloc(sizeof(char)*50);
    category = (char*)malloc(sizeof(char)*50);
    status = (char*)malloc(sizeof(char)*50);
    while (fscanf(f,"%d,%s,%s,%s,%s", &id, title, author, category, status) ==5) {
        Book book;
        book.id = id;
        strcpy(title,book.title);
        strcpy(author,book.author);
        strcpy(category,book.category);
        strcpy(status,book.status);
        insert_end(&root,book);
        printf("%d,%s,%s,%s,%s",book.id,book.title,book.author,book.category,book.status);
    }
}

void Search(Node *root, char item[50]){

}

int main(){
    Node* root=NULL;
    FILE *f = fopen("Books_data - Sheet1.csv", "r");
    char item[50];
    LoadBooks(f,root);
    for(Node*current = root; current!=NULL; current = current->next){
        Book currB = current->book;
        printf("%d,%s,%s,%s,%s",currB.id,currB.title,currB.author,currB.category,currB.status);
    }
    //printf("Hello! Welcome to the Library Book Catalogue. To look up a book, or browse a genre please enter a book title, author, or genre.");
    //scanf("%s",item);
    deallocate(&root);
}
