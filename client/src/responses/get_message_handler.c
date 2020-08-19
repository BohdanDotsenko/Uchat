#include "client.h"

#define MX_STYLES "client/src/screens/main_window/notify_on.css"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_get_message_handler(cJSON *json, t_client *client) {
    gint contact = cJSON_GetObjectItem(json, "sender_id")->valueint;

    if (client->chat_with == contact) {
        gchar *request = mx_chat_history_request(contact, client->token, 0, 20);

        mx_remove_rows(client->chat);
        mx_send_data(client->data_out, request);
        g_free(request);
    }
    else {
        if (!g_hash_table_contains(client->contacts_table,
                                   GINT_TO_POINTER(contact))) {
            gchar *request = mx_add_contact_request(contact, client->token);

            mx_send_data(client->data_out, request);
            g_free(request);
        }
        else {
            t_contact_data *node = NULL;
            node = (t_contact_data *)g_hash_table_lookup(client->contacts_table,
                                                         GINT_TO_POINTER(contact));
            GtkWidget *child = GTK_WIDGET(gtk_bin_get_child(GTK_BIN(node->row)));

            gtk_widget_set_name(child, "notify_available");
            mx_apply_styles(MX_STYLES);
        }
    }
}
