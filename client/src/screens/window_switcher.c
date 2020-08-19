#include "client.h"

void mx_window_switcher(GtkWindow *hide, GtkWindow *show) {
    gtk_widget_hide(GTK_WIDGET(hide));
    gtk_widget_show(GTK_WIDGET(show));
}
