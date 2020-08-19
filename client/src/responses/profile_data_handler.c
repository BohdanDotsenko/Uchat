#include "client.h"

static gboolean json_validator(cJSON *json) {
    cJSON *first_name = cJSON_GetObjectItemCaseSensitive(json, "first_name");
    cJSON *last_name = cJSON_GetObjectItemCaseSensitive(json, "last_name");
    cJSON *email = cJSON_GetObjectItemCaseSensitive(json, "email");

    if (first_name && cJSON_IsString(first_name) &&
        last_name && cJSON_IsString(last_name) &&
        email && cJSON_IsString(email))
            return TRUE;
    else
        return FALSE;
}

void mx_profile_data(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        const gchar *first_name = cJSON_GetObjectItem(json, "first_name")->valuestring;
        const gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
        const gchar *email = cJSON_GetObjectItem(json, "email")->valuestring;

        mx_set_entry_value(client->builder, "profile_first_name", first_name);
        mx_set_entry_value(client->builder, "profile_last_name", last_name);
        mx_set_entry_value(client->builder, "profile_email", email);
        gtk_widget_show(GTK_WIDGET(client->profile_window));
    }
    else {
        g_message("Invalid profile data response\n");
    }
}
