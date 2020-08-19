#include "client.h"

static void log_out(t_client *client) {
    gchar *request = NULL;

    request = mx_sign_out_request(client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}

GtkWidget *mx_profile_context(t_client *client) {
    GtkWidget *menu = NULL;
    GtkWidget *open_profile = NULL;
    GtkWidget *sign_out = NULL;

    menu = GTK_WIDGET(gtk_menu_new());
    open_profile = gtk_menu_item_new_with_label("Show profile");
    gtk_widget_show(open_profile);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), open_profile);
    sign_out = gtk_menu_item_new_with_label("Sign out");
    gtk_widget_show(sign_out);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), sign_out);
    g_signal_connect_swapped(G_OBJECT(open_profile), "activate",
                             G_CALLBACK(get_profile), NULL);
    g_signal_connect_swapped(G_OBJECT(sign_out), "activate",
                             G_CALLBACK(log_out), client);
    return menu;
}
