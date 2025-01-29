#ifndef HINTS_H
#define HINTS_H
#include <gtk/gtk.h>
// Array of hints for each bug
extern const char *hints[];

void show_hint(const char *hint, GtkWidget *parent_window);

const char *get_next_hint(int current_bug);
#endif
