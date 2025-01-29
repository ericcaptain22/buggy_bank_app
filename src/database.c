#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"
#include "user.h"

// Load users from a plain text file
void load_users(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open %s. Starting with an empty user list.\n", filename);
        return;
    }

    user_count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (user_count >= 10) {
            printf("Max user capacity reached. Cannot load more users.\n");
            break;
        }
        // Parse username, password, and balance
        char *username = strtok(line, ",");
        char *password = strtok(NULL, ",");
        char *balance_str = strtok(NULL, ",");
        if (username && password && balance_str) {
            strncpy(users[user_count].username, username, sizeof(users[user_count].username));
            strncpy(users[user_count].password, password, sizeof(users[user_count].password));
            users[user_count].balance = atof(balance_str);
            users[user_count].transaction_count = 0; // Initialize empty transaction history
            user_count++;
        }
    }
    fclose(file);
}

// Save users to a plain text file
void save_users(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s,%s,%.2f\n", users[i].username, users[i].password, users[i].balance);
    }
    fclose(file);
}
