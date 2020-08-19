#include "client.h"

#define MX_STYLES "client/src/screens/main_window/window_main.css"
#define MX_MAIN_WINDOW "client/src/screens/glade/main.glade"

static void controling(GtkBuilder *builder, t_client *client) {
    GtkWidget *placeholder = GTK_WIDGET(gtk_label_new("Add contact first"));

    client->contacts_table = g_hash_table_new(NULL, NULL);
    client->contacts = GTK_LIST_BOX(gtk_builder_get_object(builder,
                                                           "contacts_box"));
    client->contact_info = GTK_WIDGET(
                         gtk_builder_get_object(client->builder,
                                                "contact_info_btn"));
    gtk_list_box_set_placeholder(client->contacts, placeholder);
    gtk_widget_show(placeholder);
    mx_top_bar_control(builder, client);
    mx_chat_control(builder, client);

}

GtkWindow *mx_main_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_MAIN_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    controling(builder, client);
    g_signal_connect(window, "destroy", G_CALLBACK(shut_down), client);
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(mx_close_widget), client->chat_box);
    return window;
}
