#include "client.h"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_contact_info(cJSON *json, t_client *client) {
    const gchar *first_name = cJSON_GetObjectItem(json, "first_name")->valuestring;
    const gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
    const gchar *email = cJSON_GetObjectItem(json, "email")->valuestring;

    mx_set_entry_value(client->builder, "contact_first_name", first_name);
    mx_set_entry_value(client->builder, "contact_last_name", last_name);
    mx_set_entry_value(client->builder, "contact_email", email);
    gtk_widget_show(GTK_WIDGET(client->contact_info_window));
}
