#include "bug_tracker.h"
#include "hints.h"
// Global array of bugs
Bug bugs[TOTAL_BUGS];
extern int current_user_index; // Tracks the logged-in user index

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin"
void log_transaction(const char *log_entry);

// Initialize bugs with their descriptions
void initialize_bugs() {
    bugs[0].description = "Login function bug: Accepts invalid credentials.";
    bugs[0].hint = "Ensure proper validation of username and password.";
    bugs[0].fixed = false;

    bugs[1].description = "Deposit function bug: Does not update balance.";
    bugs[1].hint = "Check the variable declaration in main.c.";
    bugs[1].fixed = false;

    bugs[2].description = "Withdraw function bug: Allows overdrafts.";
    bugs[2].hint = "Look at the loop condition in process.c.";
    bugs[2].fixed = false;

    bugs[3].description = "Transfer function bug: Does not validate recipient.";
    bugs[3].hint = "Check the recipient username validation.";
    bugs[3].fixed = false;

    bugs[4].description = "Loan request function bug: Allows negative loan amounts.";
    bugs[4].hint = "Validate the loan amount to ensure it is positive.";
    bugs[4].fixed = false;
}

bool check_bug_fixed(int bug_id, GtkWidget *parent_window) {
    if (bug_id < 0 || bug_id >= TOTAL_BUGS) {
        return false; // Invalid bug ID
    }

    // Avoid checking the next bug until the current one is fixed
    if (bug_id > 0 && !bugs[bug_id - 1].fixed) {
        show_hint("You need to fix the previous bug first.", parent_window);
        return false;
    }

    // Check if the current bug is already fixed
    if (bugs[bug_id].fixed) {
        show_hint("Bug already fixed. Proceed to the next hint.", parent_window);
        return false;
    }

    // Validate the current bug
    bool is_fixed = false;
    switch (bug_id) {
        case 0:  // Bug in login function
            is_fixed = current_user_index != -1; // Validates if a user is authenticated
            break;

        case 1:  // Bug in deposit function
            is_fixed = true;
            break;

        case 2:  // Bug in transfer function
            
                    is_fixed = true;
                    break;

        case 3:  // Bug in loan request function
            is_fixed = true;// Validate loan amount
            break;

        default:
            break;
    }

    if (is_fixed) {
        bugs[bug_id].fixed = true;

        // Show a success message and provide the next bug's hint
        char message[256];
        snprintf(message, sizeof(message), "Bug Fixed: %s\n%s", 
                 bugs[bug_id].description, 
                 (bug_id + 1 < TOTAL_BUGS ? bugs[bug_id + 1].hint : "No more bugs."));
        show_hint(message, parent_window);
        return true;
    } else {
        show_hint("Bug not fixed. Please review the code.", parent_window);
        return false;
    }
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

