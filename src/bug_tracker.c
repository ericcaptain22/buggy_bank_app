#include "bug_tracker.h"
#include "hints.h"
// Global array of bugs
Bug bugs[TOTAL_BUGS];
extern int current_user_index; // Tracks the logged-in user index
const char *login_function_code = "if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)";
const char *deposit_function_code = "users[current_user_index].balance += amount";
const char *withdraw_function_code = "if (amount <= 0 || amount > users[current_user_index].balance)";
const char *transfer_function_code = "for (int i = 0; i < user_count; i++) { if (strcmp(users[i].username, recipient_username) == 0)";
const char *loan_request_function_code = "if (loan_amount > 0)";
const char *view_transaction_code = "FILE *file = fopen(\"assets/history.txt\", \"r\")";
const char *change_password_code = "strcpy(users[i].password, new_password); save_users(\"assets/users.txt\");";


#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin"
void log_transaction(const char *log_entry);

// Initialize bugs with their descriptions
void initialize_bugs() {
    bugs[0].description = "Login function bug: Accepts invalid credentials.";
    bugs[0].hint = "Ensure proper validation of username and password.";
    bugs[0].fixed = false;

    bugs[1].description = "Deposit function bug: Does not update balance.";
    bugs[1].hint = "Check the balance updation during deposit.";
    bugs[1].fixed = false;

    bugs[2].description = "Withdraw function bug: Allows overdrafts.";
    bugs[2].hint = "Look at the condition in the function.";
    bugs[2].fixed = false;

    bugs[3].description = "Transfer function bug: Does not validate recipient.";
    bugs[3].hint = "Check the recipient username validation.";
    bugs[3].fixed = false;

    bugs[4].description = "Loan request function bug: Allows negative loan amounts.";
    bugs[4].hint = "Validate the loan amount to ensure it is positive.";
    bugs[4].fixed = false;
}

const char *get_next_hint(int bug_id) {
    if (bug_id < 0 || bug_id >= TOTAL_BUGS - 1) {
        return "All bugs fixed!";
    }

    // Ensure we get the next bug in order
    for (int i = bug_id + 1; i < TOTAL_BUGS; i++) {
        if (!bugs[i].fixed) {
            return bugs[i].hint; // Return the hint for the next unfixed bug
        }
    }

    return "No more hints available."; // Default fallback
}

// Callback function for asynchronously displaying the hint
static gboolean show_hint_callback(gpointer message) {
    show_hint((char *)message, NULL);
    g_free(message);
    return G_SOURCE_REMOVE;  // Remove callback after execution
}

bool check_bug_fixed(int bug_id, GtkWidget *parent_window) {
    if (bug_id < 0 || bug_id >= TOTAL_BUGS) {
        return false;
    }

    // Ensure the previous bug is fixed before checking the next one
    if (bug_id > 0 && !bugs[bug_id - 1].fixed) {
        show_hint("❌ You need to fix the previous bug first.", parent_window);
        return false;
    }

    // If already fixed, just show the next hint directly
    if (bugs[bug_id].fixed) {
        char message[256];
        snprintf(message, sizeof(message), "✅ Bug already fixed. Proceed to the next hint.");
        show_hint(message, parent_window);
        return true;
    }

    bool is_fixed = false;

    if (bug_id == 0 && strstr(login_function_code, "&& strcmp(users[i].password, password) == 0")) {
        is_fixed = true;
    }

    if (bug_id == 1 && strstr(withdraw_function_code, "if (amount <= 0 || amount > users[current_user_index].balance)")) {
        is_fixed = true;
    }

    if (bug_id == 2 && strstr(transfer_function_code, "for (int i = 0; i < user_count; i++)")) {
        is_fixed = true;
    }

    if (bug_id == 3 && strstr(loan_request_function_code, "if (loan_amount > 0)")) {
        is_fixed = true;
    }

    if (bug_id == 4 && strstr(view_transaction_code, "FILE *file = fopen(\"assets/history.txt\", \"r\")")) {
        is_fixed = true;
    }

    if (bug_id == 5 && strstr(change_password_code, "save_users(\"assets/users.txt\");")) {
        is_fixed = true;
    }

    char message[256];

    if (is_fixed) {
        bugs[bug_id].fixed = true;

        // ✅ First hint box: "Bug Fixed"
        snprintf(message, sizeof(message), "✅ Bug Fixed! Look out for Next Hint");
        show_hint(message, parent_window);

        // 🔍 Second hint box: Show next hint only if another bug exists
        if (bug_id + 1 < TOTAL_BUGS) {
            snprintf(message, sizeof(message), "🔍 Next Hint: %s\n%s", bugs[bug_id + 2].description, bugs[bug_id + 2].hint);
            g_idle_add((GSourceFunc)show_hint_callback, g_strdup(message));
        }

        return true;
    } else {
        // ❌ Show only ONE hint box for a failed bug fix
        snprintf(message, sizeof(message), "❌ Bug not fixed. Review the code.");
        show_hint(message, parent_window);
        return false;
    }
}

