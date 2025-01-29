#include "hints.h"
#include <gtk/gtk.h>
#include <stdbool.h>   // Ensure bool, true, and false are available
#include "bug_tracker.h"  // Include bug_tracker.h for Bug structure and functions

int current_bug_index = 0;

void show_hint(const char *hint, GtkWidget *parent_window) {
    // Create a new dialog box
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Bug Fix Hint",                  // Title
        GTK_WINDOW(parent_window),      // Parent window
        GTK_DIALOG_MODAL,               // Modal dialog
        "Close", GTK_RESPONSE_CLOSE,    // Close button
        NULL);

    // Set dialog size
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);

    // Get content area of the dialog box
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create a scrolled window to handle large hints
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    // Create a text view to display the hint text
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    

    // Set the hint text in the text view
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), hint, -1);

    // Add the text view to the scrolled window
    gtk_container_add(GTK_CONTAINER(scroll), text_view);

    // Add the scrolled window to the content area
    gtk_box_pack_start(GTK_BOX(content_area), scroll, TRUE, TRUE, 10);

    // Show all widgets in the dialog box
    gtk_widget_show_all(dialog);

    // Run the dialog and wait for the user to respond
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Destroy the dialog after the user closes it
    gtk_widget_destroy(dialog);
}



