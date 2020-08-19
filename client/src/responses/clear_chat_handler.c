#include "client.h"

static gboolean json_validator(cJSON *json) {
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    cJSON *user_id = cJSON_GetObjectItemCaseSensitive(json, "user_id");

    if (message && cJSON_IsString(message)
        && user_id && cJSON_IsNumber(user_id)) {
            return TRUE;
    }
    else
        return FALSE;
}

void mx_clear_chat(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        gint user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
        GHashTable *contacts = client->contacts_table;
        t_contact_data *node = NULL;

        node = (t_contact_data *)g_hash_table_lookup(contacts,
                                                     GINT_TO_POINTER(user_id));
        if (user_id == client->chat_with) {
            mx_remove_rows(client->chat);
        }
    }
    else {
        g_message("Invalid clear chat response\n");
    }
}
