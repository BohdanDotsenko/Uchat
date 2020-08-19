#include "client.h"

gboolean mx_menu_callback(GtkWidget *widget, GdkEventButton *event,
                          GtkWidget *menu) {
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
        GdkEvent *bevent = (GdkEvent *)event;

        gtk_menu_popup_at_pointer(GTK_MENU(menu), bevent);
        return TRUE;
    }
    return FALSE;
    (void)widget;
}
