#include "client.h"

gchar *mx_chat_history_request(gint user_id, const gchar *token,
                               gint from, gint to) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(RQ_CHAT_HISTORY));
    cJSON_AddItemToObject(json, "token", cJSON_CreateString(token));
    cJSON_AddItemToObject(json, "contact_id", cJSON_CreateNumber(user_id));
    cJSON_AddItemToObject(json, "from", cJSON_CreateNumber(from));
    cJSON_AddItemToObject(json, "to", cJSON_CreateNumber(to));
    request = cJSON_PrintUnformatted(json);
    if (!request) {
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
