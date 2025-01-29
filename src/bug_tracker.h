#ifndef BUG_TRACKER_H
#define BUG_TRACKER_H
#include "user.h" // To access user data
#include <gtk/gtk.h>
#include <stdbool.h>

// Define Bug structure
typedef struct {
    int id;
    const char *description;  // Bug description
    const char *hint;         // Hint for the next bug
    bool fixed;               // Status: Fixed or not
} Bug;

// Total number of bugs
#define TOTAL_BUGS 4

// Declare the bugs array
extern Bug bugs[TOTAL_BUGS];

// Function declarations
void initialize_bugs();
bool check_bug_fixed(int bug_id, GtkWidget *parent_window);
const char *get_next_hint(int bug_id);
bool is_bug_fixed(int bug_id); // Declare is_bug_fixed here
#endif
