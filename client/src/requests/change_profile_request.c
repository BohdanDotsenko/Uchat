#include "client.h"

gchar *mx_change_data_request(const gchar *first_name,
                              const gchar *last_name, const gchar *email,
                              const gchar *token) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(RQ_CHANGE_PROFILE));
    cJSON_AddItemToObject(json, "token", cJSON_CreateString(token));
    cJSON_AddItemToObject(json, "first_name", cJSON_CreateString(first_name));
    cJSON_AddItemToObject(json, "last_name", cJSON_CreateString(last_name));
    cJSON_AddItemToObject(json, "email", cJSON_CreateString(email));
    request = cJSON_PrintUnformatted(json);
    if (!request) {
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
