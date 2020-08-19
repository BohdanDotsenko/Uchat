#include "client.h"

static gboolean json_validator(cJSON *json) {
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (message && cJSON_IsString(message))
            return TRUE;
    else
        return FALSE;
}

void mx_free_client_data(t_client *client) {
    mx_remove_rows(client->contacts);
    mx_remove_rows(client->chat);
    gtk_widget_hide(client->chat_box);
    client->chat_with = 0;
    if (client->contacts_table) {
        g_hash_table_destroy(client->contacts_table);
        client->contacts_table = g_hash_table_new(NULL, NULL);
    }
}

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_sign_out(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        mx_free_client_data(client);
        mx_window_switcher(client->main_window, client->phone_entering);
    }
    else {
        g_message("Invalid sign out response\n");
    }
}
