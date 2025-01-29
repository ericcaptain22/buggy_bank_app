#ifndef USER_H
#define USER_H

typedef struct {
    char username[50];
    char password[50];
    float balance;
    char transactions[10][100]; // Stores last 10 transactions
    int transaction_count;      // Tracks number of transactions
} User;

// Global user array and count
extern User users[10];
extern int user_count;

#endif
