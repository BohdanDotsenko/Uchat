#include "client.h"

static gboolean json_validator(cJSON *json) {
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    cJSON *contact = cJSON_GetObjectItemCaseSensitive(json, "contact");
    cJSON *first_name = cJSON_GetObjectItemCaseSensitive(json, "first_name");
    cJSON *last_name = cJSON_GetObjectItemCaseSensitive(json, "last_name");

    if (message && cJSON_IsString(message) &&
        contact && cJSON_IsNumber(contact) &&
        first_name && cJSON_IsString(first_name) &&
        last_name && cJSON_IsString(last_name)) {
            return TRUE;
    }
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
void mx_add_contact(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        gint contact = cJSON_GetObjectItem(json, "contact")->valueint;
        gchar *first_name = cJSON_GetObjectItem(json, "first_name")->valuestring;
        gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;

        gtk_widget_hide(client->contact_view);
        mx_show_contact_in_ui(client, first_name, last_name, contact);
        mx_window_switcher(client->add_contact_dialog, client->main_window);
        gtk_widget_grab_focus(GTK_WIDGET(client->main_window));
    }
    else {
        g_message("Invalid add contact response\n");
    }
}
