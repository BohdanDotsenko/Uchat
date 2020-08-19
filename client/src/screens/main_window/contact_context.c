#include "client.h"

static void open_contact(t_contact_data *contact) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;

    request = mx_contact_info_request(contact->id, client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}

static void open_chat(t_contact_data *contact) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;

    gtk_widget_set_visible(client->chat_box, TRUE);
    if (ABS(client->chat_with) != contact->id) {
        mx_remove_rows(GTK_LIST_BOX(client->chat));
        request = mx_chat_history_request(contact->id, client->token,  0, 20);
        mx_send_data(client->data_out, request);
        g_free(request);
    }
    else
        client->chat_with = contact->id;
}

static void remove_contact(t_contact_data *contact) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;

    request = mx_remove_contact_request(contact->id, client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}

static void remove_chat(t_contact_data *contact) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;

    request = mx_clear_chat_request(contact->id, client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}

static void init_signals(GtkWidget *first, GtkWidget *second,
                         GtkWidget *third, GtkWidget *forth, t_contact_data *contact) {
    g_signal_connect_swapped(G_OBJECT(first), "activate",
                             G_CALLBACK(open_contact), contact);
    g_signal_connect_swapped(G_OBJECT(second), "activate",
                             G_CALLBACK(open_chat), contact);
    g_signal_connect_swapped(G_OBJECT(third), "activate",
                             G_CALLBACK(remove_contact), contact);
    g_signal_connect_swapped(G_OBJECT(forth), "activate",
                             G_CALLBACK(remove_chat), contact);
}

static void init_items(GtkWidget *menu, t_contact_data *contact) {
    GtkWidget *open_contact = NULL;
    GtkWidget *open_chat = NULL;
    GtkWidget *del_contact = NULL;
    GtkWidget *clear_chat = NULL;

    open_contact = gtk_menu_item_new_with_label("Show contact");
    gtk_widget_show(open_contact);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), open_contact);
    open_chat = gtk_menu_item_new_with_label("Show chat");
    gtk_widget_show(open_chat);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), open_chat);
    del_contact = gtk_menu_item_new_with_label("Delete contact");
    gtk_widget_show(del_contact);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), del_contact);
    clear_chat = gtk_menu_item_new_with_label("Delete chat");
    gtk_widget_show(clear_chat);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), clear_chat);
    init_signals(open_contact, open_chat, del_contact, clear_chat, contact);
}

GtkWidget *mx_contact_context(t_contact_data *contact) {
    GtkWidget *menu = NULL;

    menu = GTK_WIDGET(gtk_menu_new());
    init_items(menu, contact);
    return menu;
}
