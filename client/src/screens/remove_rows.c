#include "client.h"

void mx_remove_rows(GtkListBox *listbox) {
    GList *children = NULL;
    GList *iter = NULL;

    children = gtk_container_get_children(GTK_CONTAINER(listbox));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}
