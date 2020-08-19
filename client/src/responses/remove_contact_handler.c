#include "client.h"

static gboolean json_validator(cJSON *json) {
    cJSON *contact = cJSON_GetObjectItemCaseSensitive(json, "contact");

    if (contact && cJSON_IsNumber(contact))
            return TRUE;
    else
        return FALSE;
}

void mx_remove_contact(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        gint user_id = cJSON_GetObjectItem(json, "contact")->valueint;
        GHashTable *contacts = client->contacts_table;
        t_contact_data *node = NULL;

        if (user_id == client->chat_with) {
            mx_remove_rows(client->chat);
            gtk_widget_hide(client->chat_box);
        }
        node = (t_contact_data *)g_hash_table_lookup(contacts,
                                                     GINT_TO_POINTER(user_id));
        if (node->row) {
            gtk_widget_destroy(GTK_WIDGET(node->row));
        }
        g_free(node->first_name);
        g_free(node->last_name);
        g_free(node);
        g_hash_table_remove(contacts, GINT_TO_POINTER(user_id));
    }
    else {
        g_message("Invalid remove_contact response\n");
    }
}
