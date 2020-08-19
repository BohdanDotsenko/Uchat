#include "client.h"

#define MX_STYLES "client/src/screens/main_window/notify_off.css"

static gboolean json_validator(cJSON *json) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(json, "messages");
    const cJSON *item = NULL;

    cJSON_ArrayForEach(item, data) {
        cJSON *message = cJSON_GetObjectItemCaseSensitive(item, "message");
        cJSON *date = cJSON_GetObjectItemCaseSensitive(item, "date");

        if (message && cJSON_IsString(message) && cJSON_IsNumber(date))
            return TRUE;
    }
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
void mx_get_chat_history(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        cJSON *data = cJSON_GetObjectItemCaseSensitive(json, "messages");
        cJSON *messages = NULL;
        t_contact_data *node = NULL;

        node = (t_contact_data *)g_hash_table_lookup(client->contacts_table,
            GINT_TO_POINTER(client->chat_with));
        GtkWidget *child = GTK_WIDGET(gtk_bin_get_child(GTK_BIN(node->row)));

        gtk_widget_set_name(child, "notify_disable");
        mx_apply_styles(MX_STYLES);
        cJSON_ArrayForEach(messages, data) {
            gchar *message = cJSON_GetObjectItem(messages, "message")->valuestring;
            gint date = cJSON_GetObjectItem(messages, "date")->valueint;

            mx_show_message_in_ui(client, message);
            date *= 1;
        }
    }
    else {
        g_message("Invalid chat history response\n");
    }
}
