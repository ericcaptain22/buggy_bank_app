#include <string.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "user.h"
#include "bugs.h"
#include "navigation.h"
#include "hints.h"
#include "bug_tracker.h"
#include "database.h"
#include "bug_manager.h"
#include "bug_manager.c"

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin"
GtkWidget *hint_label;
GtkWidget *message_label;
int current_user_index = -1; // Tracks the logged-in user index


void load_css() {
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    if (!gtk_css_provider_load_from_path(cssProvider, "src/style.css", NULL)) {
        g_printerr("Failed to load CSS file\n");
    }
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(cssProvider),

        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}

void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;
    const char *username = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    GtkStack *stack = GTK_STACK(entries[2]);

    // Check if the entered username and password match the admin credentials
    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        current_user_index = -1; // Admin doesn't have a user index
        gtk_stack_set_visible_child_name(stack, "admin_page");
        return;
    }

    // Check if the entered username and password match any of the regular users
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            current_user_index = i; // Set the current user index
            gtk_stack_set_visible_child_name(stack, "user_page");
            return;
        }
        
    }
    show_hint("Invalid credentials! Try again.", gtk_widget_get_toplevel(widget));
}

void on_create_account_button_clicked(GtkWidget *widget, gpointer data) {
    GtkStack *stack = GTK_STACK(data); // Properly cast to GtkStack
    gtk_stack_set_visible_child_name(stack, "create_account_page");
}

void on_create_account_confirm_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;

    const char *username = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const char *initial_deposit_str = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    float initial_deposit = atof(initial_deposit_str);

    if (strlen(username) == 0 || strlen(password) == 0) {
        g_print("Error: Username and password cannot be empty.\n");
        show_hint("Hint: Ensure all fields are filled correctly.", gtk_widget_get_toplevel(widget));
        return;
    }

    if (initial_deposit < 0) {
        g_print("Error: Initial deposit must be non-negative.\n");
        show_hint("Hint: Check the initial deposit validation.", gtk_widget_get_toplevel(widget));
        return;
    }

    create_account(username, password, initial_deposit);
    g_print("Account created successfully for %s.\n", username);
    show_hint("Account created. Return to the login page.", gtk_widget_get_toplevel(widget));
}


// Global definitions
User users[10];
int user_count = 0;

typedef struct {
    GtkWidget *amount_entry;
    GtkWidget *recipient_entry;
} TransferData;

void load_css();
void show_hint(const char *hint, GtkWidget *parent_window);
void on_login_button_clicked(GtkWidget *widget, gpointer data);
void on_create_account_button_clicked(GtkWidget *widget, gpointer data);
void on_user_action_clicked(GtkWidget *widget, gpointer data);
void on_admin_action_clicked(GtkWidget *widget, gpointer data);
void on_deposit(GtkWidget *widget, gpointer data);
void on_withdraw(GtkWidget *widget, gpointer data);
void on_transfer(GtkWidget *widget, gpointer data);
void on_request_loan(GtkWidget *widget, gpointer data);
void on_back_button_clicked(GtkWidget *widget, gpointer data);
void on_view_transactions_clicked(GtkWidget *widget, gpointer data);
void on_change_password_clicked(GtkWidget *widget, gpointer data);
void log_transaction(const char *log_entry);
void on_clear_button_clicked(GtkWidget *widget, gpointer data);
void initialize_bugs();

// Global variable for User Page
GtkWidget *user_page;
GtkWidget *deposit_button, *withdraw_button, *transfer_button, *loan_button;
GtkWidget *user_message_label; // Label to display transaction messages

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    initialize_bugs();
    // Load CSS and data
    load_css();
    load_users("assets/users.txt");

    // Main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        // Create a Header Bar with Styled Title
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Virtual Bank App");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_widget_set_name(header_bar, "header_bar");

    // Set the Header Bar for the Window
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // Create Stack
    GtkWidget *stack = gtk_stack_new();

    // --- LOGIN PAGE ---
    GtkWidget *login_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Username");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *create_account_button = gtk_button_new_with_label("Create Account");
    GtkWidget *entries[3] = {username_entry, password_entry, stack};
 
    GtkWidget *login_clear_button = gtk_button_new_with_label("Clear");

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), entries);
    g_signal_connect(create_account_button, "clicked", G_CALLBACK(on_create_account_button_clicked), stack);

    gtk_box_pack_start(GTK_BOX(login_page), username_entry, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(login_page), password_entry, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(login_page), login_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(login_page), create_account_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(login_page), login_clear_button, FALSE, FALSE, 10);

    g_signal_connect(login_clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), username_entry);
    g_signal_connect(login_clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), password_entry);


    gtk_stack_add_named(GTK_STACK(stack), login_page, "login_page");

    // --- CREATE ACCOUNT PAGE ---
    GtkWidget *create_account_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *new_username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(new_username_entry), "New Username");
    GtkWidget *new_password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(new_password_entry), "New Password");
    GtkWidget *initial_deposit_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(initial_deposit_entry), "Initial Deposit");

    GtkWidget *create_account_confirm_button = gtk_button_new_with_label("Create Account");
    GtkWidget *create_account_entries[3] = {new_username_entry, new_password_entry, initial_deposit_entry};
    g_signal_connect(create_account_confirm_button, "clicked", G_CALLBACK(on_create_account_confirm_button_clicked), create_account_entries);
    GtkWidget *back_button = gtk_button_new_with_label("Back to Login");
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), stack);

    gtk_box_pack_start(GTK_BOX(create_account_page), back_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(create_account_page), new_username_entry, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(create_account_page), new_password_entry, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(create_account_page), initial_deposit_entry, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(create_account_page), create_account_confirm_button, FALSE, FALSE, 10);

    // Add create_account_page to GtkStack
    gtk_stack_add_named(GTK_STACK(stack), create_account_page, "create_account_page");

    //---- ADMIN PAGE ---
    // Admin Page
    GtkWidget *admin_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Back Button for Admin Page
    GtkWidget *admin_back_button = gtk_button_new_with_label("Back to Login");
    g_signal_connect(admin_back_button, "clicked", G_CALLBACK(on_back_button_clicked), stack);

    // View Transactions Button
    GtkWidget *view_transactions_button = gtk_button_new_with_label("View Transactions");
    g_signal_connect(view_transactions_button, "clicked", G_CALLBACK(on_view_transactions_clicked), window);

    // Change Password Button
    GtkWidget *change_password_button = gtk_button_new_with_label("Change Password");
    g_signal_connect(change_password_button, "clicked", G_CALLBACK(on_change_password_clicked), window);

    // Add Buttons to Admin Page
    gtk_box_pack_start(GTK_BOX(admin_page), admin_back_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(admin_page), view_transactions_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(admin_page), change_password_button, FALSE, FALSE, 10);

    gtk_stack_add_named(GTK_STACK(stack), admin_page, "admin_page");



    // --- USER PAGE ---
    user_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    message_label = gtk_label_new("");  // Define the message label
    // Message Label
    user_message_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(user_page), user_message_label, FALSE, FALSE, 10);
    // Back Button for User Page
    GtkWidget *user_back_button = gtk_button_new_with_label("Back to Login");
    g_signal_connect(user_back_button, "clicked", G_CALLBACK(on_back_button_clicked), stack);
     
    gtk_box_pack_start(GTK_BOX(user_page), message_label, FALSE, FALSE, 20);  // Add it to the page

    // Add Back Button
    gtk_box_pack_start(GTK_BOX(user_page), user_back_button, FALSE, FALSE, 10);


    // Deposit Section
    GtkWidget *deposit_label = gtk_label_new("Deposit Amount:");
    GtkWidget *deposit_entry = gtk_entry_new();
    deposit_button = gtk_button_new_with_label("Deposit");
    GtkWidget *deposit_clear_button = gtk_button_new_with_label("Clear");
    gtk_box_pack_start(GTK_BOX(user_page), deposit_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), deposit_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), deposit_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(user_page), deposit_clear_button, FALSE, FALSE, 5);
    g_signal_connect(deposit_clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), deposit_entry);

    // Withdraw Section
    GtkWidget *withdraw_label = gtk_label_new("Withdraw Amount:");
    GtkWidget *withdraw_entry = gtk_entry_new();
    withdraw_button = gtk_button_new_with_label("Withdraw");
    GtkWidget *withdraw_clear_button = gtk_button_new_with_label("Clear");
    gtk_box_pack_start(GTK_BOX(user_page), withdraw_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), withdraw_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), withdraw_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(user_page), withdraw_clear_button, FALSE, FALSE, 5);
    g_signal_connect(withdraw_clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), withdraw_entry);

    // Transfer Section
    GtkWidget *transfer_label = gtk_label_new("Transfer Amount and Recipient:");
    GtkWidget *transfer_amount_entry = gtk_entry_new();
    GtkWidget *transfer_recipient_entry = gtk_entry_new();
    transfer_button = gtk_button_new_with_label("Transfer");
    GtkWidget *transfer_clear_button = gtk_button_new_with_label("Clear");
    gtk_box_pack_start(GTK_BOX(user_page), transfer_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), transfer_amount_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), transfer_recipient_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), transfer_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(user_page), transfer_clear_button, FALSE, FALSE, 5);
    g_signal_connect(transfer_clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), transfer_amount_entry);
    g_signal_connect(transfer_clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), transfer_recipient_entry);

    // Loan Section
    GtkWidget *loan_label = gtk_label_new("Loan Amount:");
    GtkWidget *loan_entry = gtk_entry_new();
    loan_button = gtk_button_new_with_label("Request Loan");
    GtkWidget *loan_clear_button = gtk_button_new_with_label("Clear");
    gtk_box_pack_start(GTK_BOX(user_page), loan_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), loan_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(user_page), loan_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(user_page), loan_clear_button, FALSE, FALSE, 5);
    g_signal_connect(loan_clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), loan_entry);


    // Signal Connections
    g_signal_connect(deposit_button, "clicked", G_CALLBACK(on_deposit), deposit_entry);
    g_signal_connect(withdraw_button, "clicked", G_CALLBACK(on_withdraw), withdraw_entry);

    TransferData *transfer_data = g_malloc(sizeof(TransferData));
    transfer_data->amount_entry = transfer_amount_entry;
    transfer_data->recipient_entry = transfer_recipient_entry;
    g_signal_connect(transfer_button, "clicked", G_CALLBACK(on_transfer), transfer_data);

    g_signal_connect(loan_button, "clicked", G_CALLBACK(on_request_loan), loan_entry);

    gtk_stack_add_named(GTK_STACK(stack), user_page, "user_page");

    // Set Stack and Show Window
    gtk_container_add(GTK_CONTAINER(window), stack);
    gtk_widget_show_all(window);
    gtk_main();

    save_users("assets/users.txt");
    return 0;
}

void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = (GtkWidget *)data;
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}


void on_back_button_clicked(GtkWidget *widget, gpointer data) {
    GtkStack *stack = GTK_STACK(data);
    gtk_stack_set_visible_child_name(stack, "login_page");
}


void on_user_action_clicked(GtkWidget *widget, gpointer data) {
    const char *action = (const char *)data;
    g_print("User action: %s\n", action);
    show_hint("Hint: Check the implementation of this function.", gtk_widget_get_toplevel(widget));
}

void on_admin_action_clicked(GtkWidget *widget, gpointer data) {
    const char *action = (const char *)data;
    g_print("Admin action: %s\n", action);
    show_hint("Hint: Check admin-specific functionality.", gtk_widget_get_toplevel(widget));
}

void on_deposit(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = (GtkWidget *)data;
    const char *amount_str = gtk_entry_get_text(GTK_ENTRY(entry));
    float amount = atof(amount_str);

    if (amount <= 0) {
        show_hint("Error: Deposit amount must be greater than 0.", gtk_widget_get_toplevel(widget));
        return;
    }

    // Intentional bug
    //users[current_user_index].balance += amount;

    log_transaction("Deposit successful.");

    // Check if the bug is fixed
    check_bug_fixed(0, gtk_widget_get_toplevel(widget)); // No need to show another hint manually
}


void on_withdraw(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = (GtkWidget *)data;
    const char *amount_str = gtk_entry_get_text(GTK_ENTRY(entry));
    float amount = atof(amount_str);

    if (amount < 0) {
        show_hint("Error: Insufficient balance.", gtk_widget_get_toplevel(widget));
        return;
    }

    users[current_user_index].balance -= amount;
    log_transaction("Withdraw successful.");

    // Check if the bug is fixed
    check_bug_fixed(1, gtk_widget_get_toplevel(widget));
}


void on_transfer(GtkWidget *widget, gpointer data) {
    TransferData *transfer_data = (TransferData *)data;
    const char *amount_str = gtk_entry_get_text(GTK_ENTRY(transfer_data->amount_entry));
    const char *recipient_username = gtk_entry_get_text(GTK_ENTRY(transfer_data->recipient_entry));
    float amount = atof(amount_str);

    if (amount <= 0 || amount > users[current_user_index].balance) {
        show_hint("Error: Invalid transfer amount.", gtk_widget_get_toplevel(widget));
        return;
    }

    // Find recipient
    for (int i = 0; i > user_count; i++) {
        if (strcmp(users[i].username, recipient_username) != 0) {
            users[current_user_index].balance += amount;
            users[i].balance -= amount;

            log_transaction("Transfer successful.");

            // Check if the bug is fixed
            check_bug_fixed(2, gtk_widget_get_toplevel(widget));
            return;
        }
    }
    show_hint("Error: Recipient not found.", gtk_widget_get_toplevel(widget));
}

void on_request_loan(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = (GtkWidget *)data;
    const char *amount_str = gtk_entry_get_text(GTK_ENTRY(entry));
    float amount = atof(amount_str);

    if (amount <= 0) {
        show_hint("Error: Loan amount must be greater than 0.", gtk_widget_get_toplevel(widget));
        return;
    }


    

    users[current_user_index].balance += amount;
    log_transaction("Loan requested successfully.");

    // Check if the bug is fixed
    check_bug_fixed(3, gtk_widget_get_toplevel(widget));
}



void on_view_transactions_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Transaction History",
        GTK_WINDOW(data), // Parent window
        GTK_DIALOG_MODAL,
        "Close", GTK_RESPONSE_CLOSE,
        NULL);

    // Set dialog size
    gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 400);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);
    gtk_box_pack_start(GTK_BOX(content_area), scroll, TRUE, TRUE, 10);

    // Read transactions from history.txt
    FILE *file = fopen("assets/history.txt", "r");
    if (!file) {
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), "Error: Could not open history.txt.", -1);
    } else {
        char buffer[4096] = "";
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            strcat(buffer, line);
        }
        fclose(file);
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), buffer, -1);
    }

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}



void on_change_password_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Change User Password",
        GTK_WINDOW(data), // Set parent window
        GTK_DIALOG_MODAL,
        "Change", GTK_RESPONSE_OK,
        "Cancel", GTK_RESPONSE_CANCEL,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *username_label = gtk_label_new("Username:");
    GtkWidget *username_entry = gtk_entry_new();
    GtkWidget *password_label = gtk_label_new("New Password:");
    GtkWidget *password_entry = gtk_entry_new();

    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    gtk_box_pack_start(GTK_BOX(content_area), username_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), username_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), password_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), password_entry, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
        const char *new_password = gtk_entry_get_text(GTK_ENTRY(password_entry));

        // Check if the user exists and update the password
        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].username, username) == 0) {
                strcpy(users[i].password, new_password);
                save_users("assets/users.txt");
                gtk_widget_destroy(dialog);
                GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Password updated successfully!");
                gtk_dialog_run(GTK_DIALOG(success_dialog));
                gtk_widget_destroy(success_dialog);
                return;
            }
        }

        GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: User not found!");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }

    gtk_widget_destroy(dialog);
}


void log_transaction(const char *log_entry) {
    FILE *file = fopen("assets/history.txt", "a");
    if (!file) {
        g_print("Error: Could not open history.txt for logging.\n");
        return;
    }
    fprintf(file, "%s\n", log_entry);
    fclose(file);
}

void mark_bug_fixed(int bug_id) {
    FILE *file = fopen("assets/bug_status.txt", "a");
    if (!file) {
        g_print("Error: Could not open bug_status.txt.\n");
        return;
    }
    fprintf(file, "Bug %d: Fixed\n", bug_id);
    fclose(file);
}
