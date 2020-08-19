#include "client.h"

#define MX_DIALOG_INFO "client/src/screens/glade/contact_info.glade"
#define MX_STYLES "client/src/screens/main_window/dialog.css"

static void close_callback(GtkWidget *button, GtkWidget *widget) {
    gtk_widget_hide(widget);
    (void)button;
}

GtkWindow *mx_contact_info_window(t_client *client) {
    GError *error = NULL;
    GtkWindow *window = NULL;
    GtkButton *close = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(client->builder, MX_DIALOG_INFO, &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(client->builder, "window"));
    close = GTK_BUTTON(gtk_builder_get_object(client->builder, "close_btn"));
    gtk_window_set_transient_for(window, client->main_window);
    g_signal_connect(close, "clicked", G_CALLBACK(close_callback), GTK_WIDGET(window));
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(mx_close_widget), window);
    return window;
}
