#include "client.h"

void mx_scroll_to_bottom(t_client *client) {
    GtkAdjustment *adj = NULL;
    GtkWidget* scroll = client->scroll;

    adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scroll));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
}
