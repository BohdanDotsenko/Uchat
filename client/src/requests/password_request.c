#include "client.h"

gchar *mx_password_request(const gchar *phone) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;
    gint type = RQ_GENERATE_PASS;

    cJSON_AddItemToObject(json, "request_type", cJSON_CreateNumber(type));
    cJSON_AddItemToObject(json, "phone", cJSON_CreateString(phone));
    request = cJSON_PrintUnformatted(json);
    if (!request) {
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
