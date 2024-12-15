#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct book {
    int id;
    char *title;
    char *author;
    char *category;
    char *status;
} Book;

typedef struct Node {
    Book book;
    struct Node *next;
} Node;

void deallocate(Node **root) {
    Node *curr = *root;
    while (curr != NULL) {
        Node *aux = curr;
        free(aux->book.title);
        free(aux->book.author);
        free(aux->book.category);
        free(aux->book.status);
        curr = curr->next;
        free(aux);
    }
    *root = NULL;
}

void insert_end(Node **root, Book value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(1);
    }

    new_node->book.id = value.id;
    new_node->book.title = strdup(value.title);
    new_node->book.author = strdup(value.author);
    new_node->book.category = strdup(value.category);
    new_node->book.status = strdup(value.status);
    new_node->next = NULL;

    if (*root == NULL) {
        *root = new_node;
        return;
    }

    Node *curr = *root;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
}

void LoadBooks(FILE *f, Node **root) {
    if (f == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Book book;
        book.title = malloc(50);
        book.author = malloc(50);
        book.category = malloc(50);
        book.status = malloc(50);

        if (sscanf(line, "%d,%49[^,],%49[^,],%49[^,],%49[^]", &book.id, book.title, book.author, book.category, book.status) == 5) {
            insert_end(root, book);
        } else {
            free(book.title);
            free(book.author);
            free(book.category);
            free(book.status);
        }
    }
}

void to_lowercase(char *dest, const char *src) {
    while (*src) {
        *dest++ = tolower(*src++);
    }
    *dest = '\0';
}


void Search(Node *root) {
    while (1) {
        char searchType[10];
        char searchTerm[50];
        char searchTermLower[50];
        printf("How do you want to search? (title/author/genre): ");
        scanf(" %9s", searchType);

        printf("Enter your search term: ");
        scanf(" %[^\n]s", searchTerm);
        to_lowercase(searchTermLower, searchTerm);

        Node *current = root;
        int found = 0, count = 0;

        Node *matches[100];

        while (current != NULL) {
            char field[50];
            if (strcmp(searchType, "title") == 0) {
                to_lowercase(field, current->book.title);
            } else if (strcmp(searchType, "author") == 0) {
                to_lowercase(field, current->book.author);
            } else if (strcmp(searchType, "genre") == 0) {
                to_lowercase(field, current->book.category);
            } else {
                printf("Invalid search type.\n");
                return;
            }

            if (strstr(field, searchTermLower)) {
                matches[count++] = current;
                printf("%d. %s by %s (Genre: %s, Status: %s)\n", count, current->book.title, current->book.author, current->book.category, current->book.status);
                found = 1;
            }
            current = current->next;
        }

        if (!found) {
            printf("No books found matching the search criteria.\n");
        } else if (count == 1) {
            Node *selectedBook = matches[0];
            char action;

            printf("One book found: '%s' by %s.\n", selectedBook->book.title, selectedBook->book.author);
            printf("Do you want details (d) or check out (c)? ");
            scanf(" %c", &action);

            if (action == 'd') {
                printf("Details:\n");
                printf("ID: %d\nTitle: %s\nAuthor: %s\nCategory: %s\nStatus: %s\n",
                       selectedBook->book.id, selectedBook->book.title, selectedBook->book.author, selectedBook->book.category, selectedBook->book.status);

                printf("Do you want to check out the book? (y/n): ");
                char checkoutChoice;
                scanf(" %c", &checkoutChoice);

                if (checkoutChoice == 'y') {
                    if (strcmp(selectedBook->book.status, "available") == 0) {
                        printf("The book is available. Checking it out...\n");
                        free(selectedBook->book.status);
                        selectedBook->book.status = strdup("issued");
                    }
                }
            } else if (action == 'c') {
                if (strcmp(selectedBook->book.status, "available") == 0) {
                    printf("The book is available. Checking it out...\n");
                    free(selectedBook->book.status);
                    selectedBook->book.status = strdup("issued");
                }
            } else {
                printf("Invalid action.\n");
            }
        } else {
            int choice;
            printf("Select a book by number (or enter 0 to cancel): ");
            scanf("%d", &choice);

            if (choice > 0 && choice <= count) {
                Node *selectedBook = matches[choice - 1];
                char action;

                printf("You selected '%s' by %s.\n", selectedBook->book.title, selectedBook->book.author);
                printf("Do you want details (d) or check out (c)? ");
                scanf(" %c", &action);

                if (action == 'd') {
                    printf("Details:\n");
                    printf("ID: %d\nTitle: %s\nAuthor: %s\nCategory: %s\nStatus: %s\n",
                           selectedBook->book.id, selectedBook->book.title, selectedBook->book.author, selectedBook->book.category, selectedBook->book.status);

                    printf("Do you want to check out the book? (y/n): ");
                    char checkoutChoice;
                    scanf(" %c", &checkoutChoice);

                    if (checkoutChoice == 'y') {
                        if (strcmp(selectedBook->book.status, "available") == 0) {
                            printf("The book is available. Checking it out...\n");
                            free(selectedBook->book.status);
                            selectedBook->book.status = strdup("issued");
                        }
                    }
                } else if (action == 'c') {
                    if (strcmp(selectedBook->book.status, "available") == 0) {
                        printf("The book is available. Checking it out...\n");
                        free(selectedBook->book.status);
                        selectedBook->book.status = strdup("issued");
                    }
                } else {
                    printf("Invalid action.\n");
                }
            } else if (choice == 0) {
                printf("Search canceled.\n");
            } else {
                printf("Invalid selection.\n");
            }
        }

        printf("Do you want to search again? (y/n): ");
        char repeat;
        scanf(" %c", &repeat);

        if (repeat == 'n') {
            printf("Exiting search.\n");
            break;
        }
    }
}


int main() {
    Node *root = NULL;
    FILE *f = fopen("Books_data - Sheet1.csv", "r");
    if (f == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    LoadBooks(f, &root);
    fclose(f);

    printf("Loaded books:\n");
    for (Node *current = root; current != NULL; current = current->next) {
        Book currB = current->book;
        printf("%d,%s,%s,%s,%s\n", currB.id, currB.title, currB.author, currB.category, currB.status);
    }

    Search(root);

    deallocate(&root);
    return 0;
}
