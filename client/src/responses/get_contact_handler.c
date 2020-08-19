#include "client.h"

static void contact_info(GtkWidget *widget, gpointer user_id) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;
    gint id = GPOINTER_TO_INT(user_id);

    request = mx_contact_info_request(id, client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
    (void)widget;
}

static void open_exist_chat(t_client *client, gint user_id) {
    GHashTable *contacts = client->contacts_table;
    t_contact_data *node = (t_contact_data *)g_hash_table_lookup(contacts,
                                                 GINT_TO_POINTER(user_id));
    GtkButton *header = GTK_BUTTON(client->contact_info);
    gchar *label = g_strjoin(" ", node->last_name, node->first_name, NULL);;
    gchar *request = NULL;
    g_print("open_chat\n");

    mx_remove_rows(GTK_LIST_BOX(client->chat));
    client->chat_with = user_id;

    request = mx_chat_history_request(client->chat_with, client->token, 0, 20);
    gtk_list_box_select_row(client->contacts, GTK_LIST_BOX_ROW(node->row));
    gtk_widget_set_visible(client->chat_box, TRUE);
    gtk_button_set_label(header, label);
    g_signal_connect(header, "clicked", G_CALLBACK(contact_info), GINT_TO_POINTER(user_id));
    mx_send_data(client->data_out, request);
    g_free(request);
}

static void add_contact(GtkWidget *widget, gpointer user_id) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;
    gint new_contact = GPOINTER_TO_INT(user_id);

    gtk_widget_hide(widget);
    mx_set_entry_value(client->builder, "dialog_search", "\0");
    if (!g_hash_table_contains(client->contacts_table, user_id)) {
        request = mx_add_contact_request(new_contact, client->token);
        mx_send_data(client->data_out, request);
        g_free(request);
    }
    else if (client->chat_with != new_contact)
        open_exist_chat(client, new_contact);
    mx_window_switcher(client->add_contact_dialog, client->main_window);
}

static gboolean json_validator(cJSON *json) {
    cJSON *user_id = cJSON_GetObjectItemCaseSensitive(json, "user_id");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "first_name");
    cJSON *last_name = cJSON_GetObjectItemCaseSensitive(json, "last_name");

    if (user_id && cJSON_IsNumber(user_id) &&
        name && cJSON_IsString(name) &&
        last_name && cJSON_IsString(last_name))
            return TRUE;
    else
        return FALSE;
}
/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_get_contact(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        gint user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
        gchar *name = cJSON_GetObjectItem(json, "first_name")->valuestring;
        gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
        gchar *label = g_strjoin(" ", last_name, name, NULL);

        gtk_widget_hide(client->contact_view);
        gtk_button_set_label(GTK_BUTTON(client->contact_view), label);
        g_signal_connect(client->contact_view, "clicked", G_CALLBACK(add_contact),
                         GINT_TO_POINTER(user_id));
        gtk_widget_show(client->contact_view);
    }
    else {
        g_message("Invalid get contactd response\n");
    }
}
