#ifndef BUG_MANAGER_H
#define BUG_MANAGER_H
#include <gtk/gtk.h>
#include <stdbool.h>

bool check_bug_fixed(int bug_id, GtkWidget *parent_window);

#endif // BUG_MANAGER_H
