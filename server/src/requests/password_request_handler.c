#include "server.h"

static gchar *password_send_response(void) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "Password has successfully sent.";
    gchar *response = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_PASSWORD_SENT));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

static void send_response(gint status, t_client *client) {
    gchar *response = NULL;

    if (!status)
        response = password_send_response();
    else
        response = mx_send_error_response(ER_SENT_PASS,
                                          "Sending password error");
    mx_send_data(client->data_out, response);
    g_free(response);
}

void mx_password_request_handler(cJSON *root, t_client *client) {
    if (cJSON_GetObjectItem(root, "phone")) {
        gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;
        gint status = 0;
        gchar *body = NULL;
        gchar *password = mx_generate_password();

        client->password = g_compute_checksum_for_string(
                         G_CHECKSUM_SHA256, password, strlen(password));
        // body = mx_recovery_body("name", password);
        // status = mx_send_mail("examle@gmail.com", body);
        body = mx_create_sms_body(phone, password);
        status = mx_send_sms(body);
        send_response(status, client);
        g_free(body);
        g_free(password);
        return;
    }
    g_warning("Not valid request\n");
}
