#include "user.h"
#include "bugs.h"
#include <stdio.h>
#include <string.h>

// Login validation (Bug: Doesn't check password properly)
int validate_user(const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 1; // Bug: Ignores password validation
        }
    }
    return 0;
}

// Other functions (e.g., deposit, withdraw, transfer) remain unchanged


// Deposit money (Bug: Doesn't update balance)
void deposit(User *user, float amount) {
    printf("Deposited %.2f successfully!\n", amount);
    // Bug: Balance not updated
}

// Withdraw money (Bug: Allows withdrawal even if balance is insufficient)
void withdraw(User *user, float amount) {
    user->balance -= amount; // Bug: No balance check
    printf("Withdrew %.2f successfully!\n", amount);
}

// Transfer funds (Bug: Receiver balance not updated)
void transfer(User *sender, User *receiver, float amount) {
    if (sender->balance < amount) {
        printf("Insufficient funds.\n");
        return;
    }
    sender->balance -= amount;
    // Bug: Receiver balance not updated
    printf("Transferred %.2f to %s.\n", amount, receiver->username);
}

// Create account (Bug: Allows duplicate usernames)
void create_account(const char *username, const char *password, float initial_deposit) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Account with username %s already exists.\n", username);
            return; // Bug: Doesn't prevent duplicate usernames
        }
    }

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    users[user_count].balance = initial_deposit;
    user_count++;

    printf("Account created for %s with balance %.2f.\n", username, initial_deposit);
}

// View transaction history (Bug: Doesn't display transactions correctly)
void view_transaction_history(User *user) {
    printf("Transaction history for %s:\n", user->username);
    for (int i = 0; i < user->transaction_count; i++) {
        printf("%s\n", user->transactions[i]);
    }
    // Bug: Doesn't handle empty transaction history
}

// Change password (Bug: Doesn't verify old password)
void change_password(User *user, const char *new_password) {
    strcpy(user->password, new_password); // Bug: No verification of old password
    printf("Password changed successfully.\n");
}

// Request loan (Bug: Approves loans for negative amounts)
void request_loan(User *user, float amount) {
    if (amount <= 0) {
        printf("Loan approved for %.2f.\n", amount); // Bug: Approves negative loans
        return;
    }
    user->balance += amount;
    printf("Loan of %.2f approved. New balance: %.2f\n", amount, user->balance);
}
