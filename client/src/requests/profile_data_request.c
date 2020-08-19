#include "client.h"

gchar *mx_profile_data_request(const gchar *token) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;


    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(RQ_PROFILE_DATA));
    cJSON_AddItemToObject(json, "token", cJSON_CreateString(token));
    request = cJSON_PrintUnformatted(json);
    if (!request) {
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
