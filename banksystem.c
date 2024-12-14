#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    int accountNumber;
    char name[100];
    double balance;
} Account;
typedef struct Node {
    Account *account;
    struct Node *next;
} Node;
Node *head = NULL;
Account *createAccount(int accountNumber, const char *name, double initialBalance);
void addAccount(Account *account);
Account *findAccount(int accountNumber);
void deposit(int accountNumber, double amount);
void withdraw(int accountNumber, double amount);
void transfer(int fromAccountNumber, int toAccountNumber, double amount);
void displayAccounts();
void loadAccountsFromFile(const char *filename);
void saveAccountsToFile(const char *filename);
void deleteAccount(int accountNumber);
int authenticate();
int main() {
    loadAccountsFromFile("accounts.txt");

    if (!authenticate()) {
        return 0;
    }

    int choice;
    do {
        printf("\nBank Account System\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. Display Accounts\n");
        printf("6. Delete Account\n");
        printf("7. Exit\n");
        printf("Enter your choice (As the number corresponding to the option): ");


        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            printf("Invalid input. Please enter a valid option.\n");
            continue;
        }

        int accountNumber, toAccountNumber;
        double amount;
        char name[100];

        switch (choice) {
                case 1:
                    printf("Enter account number: ");
                    while (scanf("%d", &accountNumber) != 1) {
                        printf("Invalid input. Enter a valid account number: ");
                        while (getchar() != '\n'); 
                    }
                    getchar();
                    
                    printf("Enter name: ");
                    scanf("%s", name);
                    getchar();
                    
                    printf("Enter initial balance: ");
                    char buffer[100];
                    while (1) {
                        
                        fgets(buffer, sizeof(buffer), stdin);

                        
                        if (sscanf(buffer, "%lf", &amount) == 1 && amount >= 0) {
                            break; 
                        } else {
                            printf("Invalid input. Enter a valid balance: ");
                        }
                    }

                    addAccount(createAccount(accountNumber, name, amount));
                    break;

            case 2:
                printf("Enter account number: ");
                while (scanf("%d", &accountNumber) != 1) {
                    printf("Invalid input. Enter a valid account number: ");
                    while (getchar() != '\n'); 
                }
                printf("Enter deposit amount: ");
                while (scanf("%lf", &amount) != 1 || amount <= 0) {
                    printf("Invalid input. Enter a valid deposit amount: ");
                    while (getchar() != '\n'); 
                }
                deposit(accountNumber, amount);
                break;

            case 3:
                printf("Enter account number: ");
                while (scanf("%d", &accountNumber) != 1) {
                    printf("Invalid input. Enter a valid account number: ");
                    while (getchar() != '\n'); 
                }
                printf("Enter withdrawal amount: ");
                while (scanf("%lf", &amount) != 1 || amount <= 0) {
                    printf("Invalid input. Enter a valid withdrawal amount: ");
                    while (getchar() != '\n'); 
                }
                withdraw(accountNumber, amount);
                break;

            case 4:
                printf("Enter sender account number: ");
                while (scanf("%d", &accountNumber) != 1) {
                    printf("Invalid input. Enter a valid account number: ");
                    while (getchar() != '\n');
                }
                printf("Enter recipient account number: ");
                while (scanf("%d", &toAccountNumber) != 1) {
                    printf("Invalid input. Enter a valid account number: ");
                    while (getchar() != '\n');
                }
                printf("Enter transfer amount: ");
                while (scanf("%lf", &amount) != 1 || amount <= 0) {
                    printf("Invalid input. Enter a valid transfer amount: ");
                    while (getchar() != '\n'); 
                }
                transfer(accountNumber, toAccountNumber, amount);
                break;

            case 5:
                displayAccounts();
                break;

            case 6:
                printf("Enter account number to delete: ");
                while (scanf("%d", &accountNumber) != 1) {
                    printf("Invalid input. Enter a valid account number: ");
                    while (getchar() != '\n'); 
                }
                deleteAccount(accountNumber);
                break;

            case 7:
                saveAccountsToFile("accounts.txt");
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
Account *createAccount(int accountNumber, const char *name, double initialBalance) {
    Account *newAccount = (Account *)malloc(sizeof(Account));
    if (!newAccount) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newAccount->accountNumber = accountNumber;
    strcpy(newAccount->name, name);
    newAccount->balance = initialBalance;
    return newAccount;
}
void addAccount(Account *account) {
    if (!account) {
        printf("Invalid account data. Cannot add account.\n");
        return;
    }
    if (findAccount(account->accountNumber)) {
        free(account); 
        return;
    }
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->account = account;
    newNode->next = head;
    head = newNode;
}
Account *findAccount(int accountNumber) {
    Node *current = head;
    while (current) {
        if (current->account->accountNumber == accountNumber) {
            return current->account;
        }
        current = current->next;
    }
    return NULL;
}
void deposit(int accountNumber, double amount) {
    Account *account = findAccount(accountNumber);
    if (account) {
        account->balance += amount;
        printf("Deposited %.2lf to account %d. New balance: %.2lf\n", amount, accountNumber, account->balance);
    } else {
        printf("Account not found!\n");
    }
}
void withdraw(int accountNumber, double amount) {
    Account *account = findAccount(accountNumber);
    if (account) {
        if (account->balance >= amount) {
            account->balance -= amount;
            printf("Withdrew %.2lf from account %d. New balance: %.2lf\n", amount, accountNumber, account->balance);
        } else {
            printf("Insufficient funds!\n");
        }
    } else {
        printf("Account not found!\n");
    }
}
void transfer(int fromAccountNumber, int toAccountNumber, double amount) {
    if (fromAccountNumber == toAccountNumber) {
        printf("Cannot transfer to the same account.\n");
        return;
    }
    Account *fromAccount = findAccount(fromAccountNumber);
    Account *toAccount = findAccount(toAccountNumber);
    if (fromAccount && toAccount) {
        if (fromAccount->balance >= amount) {
            fromAccount->balance -= amount;
                        toAccount->balance += amount;
            printf("Transferred %.2lf from account %d to account %d. New balances: Sender: %.2lf, Recipient: %.2lf\n", 
                   amount, fromAccountNumber, toAccountNumber, fromAccount->balance, toAccount->balance);
        } else {
            printf("Insufficient funds in sender's account!\n");
        }
    } else {
        printf("One or both accounts not found!\n");
    }
}
#include <stdio.h>
#include <stdlib.h>

#include <string.h> 
int authenticate() {
    const char correctUsername[] = "admin";
    const char correctPassword[] = "12345";
    char username[50];
    char password[50];
    int attempts = 3;

    while (attempts > 0) {
        printf("Enter username: ");
        scanf("%s", username);

        printf("Enter password: ");
        scanf("%s", password);

    
        if (strcmp(username, correctUsername) == 0 && strcmp(password, correctPassword) == 0) {
            printf("Login successful.\n");
            return 1; 
        } else {
            attempts--;
            printf("Incorrect login information. Access denied.\n");
            if (attempts > 0) {
                printf("You have %d attempt(s) left.\n", attempts);
            } else {
                printf("Maximum attempts reached. Exiting program.\n");
                return 0;
            }
        }
    }
    return 0;
}


void deleteAccount(int accountNumber) {
    Node *current = head, *prev = NULL;
    while (current) {
        if (current->account->accountNumber == accountNumber) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            free(current->account);
            free(current);
            printf("Account %d deleted successfully.\n", accountNumber);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Account not found.\n");
}

void displayAccounts() {
    Node *current = head;
    if (!current) {
        printf("No accounts found.\n");
        return;
    }
    while (current) {
        printf("Account Number: %d, Name: %s, Balance: %.2lf\n", 
               current->account->accountNumber, 
               current->account->name, 
               current->account->balance);
        current = current->next;
    }
}

void loadAccountsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No existing account file found.\n");
        return;
    }
    int accountNumber;
    char name[100];
    double balance;
    while (fscanf(file, "%d %s %lf", &accountNumber, name, &balance) == 3) {
        addAccount(createAccount(accountNumber, name, balance));
    }
    fclose(file);
    printf("Accounts loaded successfully.\n");
}

void saveAccountsToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error saving accounts to file.\n");
        return;
    }
    Node *current = head;
    while (current) {
        fprintf(file, "%d %s %.2lf\n", 
                current->account->accountNumber, 
                current->account->name, 
                current->account->balance);
        current = current->next;
    }
    fclose(file);
    printf("Accounts saved successfully.\n");
}

