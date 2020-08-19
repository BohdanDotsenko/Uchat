#include "server.h"

gchar *mx_send_error_response(gint type, gchar *message) {
    cJSON *json = cJSON_CreateObject();
    gchar *error_res = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(type));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    error_res = cJSON_PrintUnformatted(json);
    if (!error_res) {
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return error_res;
}
