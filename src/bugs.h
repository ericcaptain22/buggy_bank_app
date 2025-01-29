#ifndef BUGS_H
#define BUGS_H

#include "user.h"

// Prototypes for buggy functions
int validate_user(const char *username, const char *password);
void deposit(User *user, float amount);
void withdraw(User *user, float amount);
void transfer(User *sender, User *receiver, float amount);
void create_account(const char *username, const char *password, float initial_deposit);
void view_transaction_history(User *user);
void change_password(User *user, const char *new_password);
void request_loan(User *user, float amount);

#endif
