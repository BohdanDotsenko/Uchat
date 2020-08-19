#include "client.h"

gchar *mx_auth_request(const gchar *phone, const gchar *password) {
    t_client *client = *mx_get_client();
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;
    gint type = RQ_AUTH;
    gchar *hash = g_compute_checksum_for_string(
                G_CHECKSUM_SHA256, password, strlen(password));

    cJSON_AddItemToObject(json, "request_type", cJSON_CreateNumber(type));
    cJSON_AddItemToObject(json, "phone", cJSON_CreateString(phone));
    cJSON_AddItemToObject(json, "password", cJSON_CreateString(hash));
    client->token = mx_create_token((gchar *)phone, (gchar *)hash);
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    g_free(hash);
    return request;
}
