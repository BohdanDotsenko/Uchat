#include "server.h"

static gboolean json_validator(cJSON *root, t_client *client) {
    cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");

    if (token && cJSON_IsString(token)) {
        if (!g_strcmp0(token->valuestring, client->token)) {
            return TRUE;
        }
        else
            return FALSE;
    }
    else
        return FALSE;
}

void del_client_data(t_client *client) {
    GHashTable *online_users = *mx_get_online_users();

    g_hash_table_remove(online_users, &client->uid);
    g_free(client->token);
    g_free(client->password);
    client->uid = 0;
}

void mx_sign_out_request_handler(cJSON *root, t_client *client) {
    if (json_validator(root, client)) {
        cJSON *json = cJSON_CreateObject();
        gchar *response = NULL;

        del_client_data(client);
        cJSON_AddNumberToObject(json, "response_type", RS_SIGN_OUT);
        cJSON_AddStringToObject(json, "message", "Successfully sign out");
        response = cJSON_PrintUnformatted(json);
        if (!response) {
            g_warning("Failed to print make request.\n");
        }
        cJSON_Delete(json);
        mx_send_data(client->data_out, response);
        g_free(response);
        return;
    }
    g_warning("Not valid request\n");
}
