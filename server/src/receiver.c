#include "server.h"
#define REQUEST_HANDLER_SIZE 14

static gboolean json_validator(const cJSON *root, t_client *client) {
    cJSON *req_type = cJSON_GetObjectItemCaseSensitive(root, "request_type");

    if (!root || !req_type || !cJSON_IsNumber(req_type))
        return FALSE;
    if (req_type->valueint > REQUEST_HANDLER_SIZE)
        return FALSE;
    if (req_type->valueint > RQ_SIGN_UP) {
        cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");

        if (token && cJSON_IsString(token)
            && !g_strcmp0(client->token, token->valuestring))
            return TRUE;
        else
            return FALSE;
    }
    return TRUE;
}

void (*const request_handler[REQUEST_HANDLER_SIZE])() = {
    mx_password_request_handler,
    mx_auth_request_handler,
    mx_sign_up_request_handler,
    mx_get_contact_handler,
    mx_add_contact_handler,
    mx_get_contacts_list,
    mx_send_message,
    mx_get_chat_history,
    mx_sign_out_request_handler,
    mx_remove_contact_handler,
    mx_delete_chat,
    mx_get_profile_info,
    mx_change_profile,
    mx_get_contact_info,
};

void mx_receiver(gchar *data, t_client *client) {
    cJSON *root = cJSON_Parse(data);

    g_message("data from client - %s\n", data);
    if (json_validator(root, client)) {
        cJSON *req_type = cJSON_GetObjectItem(root, "request_type");

        request_handler[req_type->valueint](root, client);
        g_free(data);
        cJSON_Delete(root);
    }
    else {
        g_warning("Invalid request, try again!\n");
    }
}
