#include "client.h"

static void get_user_id(GtkWidget *widget, t_client *client) {
    GtkButton *header = GTK_BUTTON(client->contact_info);
    gchar *label = (gchar *)gtk_button_get_label(GTK_BUTTON(widget));
    GtkWidget *parent_row = gtk_widget_get_parent(widget);

    gtk_list_box_select_row(client->contacts, GTK_LIST_BOX_ROW(parent_row));
    gtk_widget_set_visible(client->chat_box, TRUE);
    gtk_button_set_label(header, label);
}

gboolean get_chat(GtkWidget *widget, GdkEventButton *event,
                  t_contact_data *contact) {
    t_client *client = *mx_get_client();
    GtkWidget *chat = client->chat_box;

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) {
        if (client->chat_with != contact->id) {
            gchar *request = NULL;

            client->chat_with = contact->id;
            get_user_id(widget, client);
            mx_remove_rows(GTK_LIST_BOX(client->chat));
            gtk_widget_set_visible(chat, TRUE);
            request = mx_chat_history_request(contact->id, client->token, 0, 20);
            mx_send_data(client->data_out, request);
            gtk_widget_show(chat);
            g_free(request);
        }
        return TRUE;
    }
    return FALSE;
}

static t_contact_data *setup_contact_struct(gchar *first_name,
                                            gchar *last_name, gint user_id) {
    t_contact_data *contact = g_new(t_contact_data, 1);

    contact->row = GTK_WIDGET(gtk_list_box_row_new());
    contact->first_name = g_strdup(first_name);
    contact->last_name = g_strdup(last_name);
    contact->id = user_id;
    return contact;
}

void mx_show_contact_in_ui(t_client *client, gchar *first_name,
                           gchar *last_name, gint user_id) {
    if (!g_hash_table_contains(client->contacts_table,
                               GINT_TO_POINTER(user_id))) {
        t_contact_data *new_contact = NULL;
        gchar *label = g_strjoin(" ", last_name, first_name, NULL);
        GtkWidget *contact = gtk_button_new_with_label(label);

        gtk_widget_set_name(contact, "contact");
        new_contact = setup_contact_struct(first_name, last_name, user_id);
        new_contact->popup = mx_contact_context(new_contact);
        gtk_container_add(GTK_CONTAINER(new_contact->row), contact);
        gtk_container_add(GTK_CONTAINER(client->contacts), new_contact->row);
        gtk_widget_show_all(new_contact->row);
        g_hash_table_insert(client->contacts_table,
                            GINT_TO_POINTER(user_id), new_contact);
        g_signal_connect(G_OBJECT(contact), "button-press-event",
                    G_CALLBACK(get_chat), new_contact);
        g_signal_connect(G_OBJECT(contact), "button-press-event",
                    G_CALLBACK(mx_menu_callback), new_contact->popup);
    }
}
