#include "bug_manager.h"
#include "gtk/gtk.h"
#include "bug_tracker.h"
#include "utils.h"

// Function to show appropriate hints based on the bug status
void check_and_show_bug_hint(GtkWidget *widget) {
    if (!bugs[0].fixed) {
        show_hint("Bug Hint (Login Function): \nThe user is not validated properly when clicking the login button.", gtk_widget_get_toplevel(widget));
    } else {
        show_hint("Invalid credentials! Try again.", gtk_widget_get_toplevel(widget));
    }
    if (!bugs[1].fixed)
    {
        show_hint("Bug Hint:\n Deposit function does not update balance of each user.", gtk_widget_get_toplevel(widget));
    }
    if (!bugs[2].fixed)
    {
        show_hint("Bug Hint: \n Withdraw function allows overdrafts.", gtk_widget_get_toplevel(widget));
    }
}

