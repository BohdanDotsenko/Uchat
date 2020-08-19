#include "server.h"

void mx_sign_up_request_handler(cJSON *root, t_client *client) {
    if (cJSON_GetObjectItem(root, "first_name") &&
        cJSON_GetObjectItem(root, "last_name") &&
        cJSON_GetObjectItem(root, "email")) {
        gchar *response = NULL;
        sqlite3 *db = *(mx_get_db());
        gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;

        mx_add_user_to_bd(root, client, db);
        response = mx_auth_send_response(client, phone);
        mx_send_data(client->data_out, response);
        g_free(response);
        return;
    }
    g_warning("Not valid request\n");
}
