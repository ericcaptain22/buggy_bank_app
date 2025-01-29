#include "navigation.h"

void switch_page(GtkStack *stack, const char *page_name) {
    gtk_stack_set_visible_child_name(stack, page_name);
}
