#include "client.h"

gboolean mx_close_widget(GtkWidget *widget, GdkEventKey *event,
                         GtkWidget *to_close) {
    t_client *client = *mx_get_client();
    
    if (event->keyval == GDK_KEY_Escape && client->chat_with > 0) {

        client->chat_with *= -1;

        gtk_widget_set_visible(to_close, FALSE);
        return TRUE;
    }
    return FALSE;
    (void)widget;
}
