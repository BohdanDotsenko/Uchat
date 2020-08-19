#include "client.h"

gchar *mx_clear_chat_request(gint user_id, const gchar *token) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(RQ_CLEAR_CHAT));
    cJSON_AddItemToObject(json, "user_id", cJSON_CreateNumber(user_id));
    cJSON_AddItemToObject(json, "token", cJSON_CreateString(token));
    request = cJSON_PrintUnformatted(json);
    if (!request) {
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
