#include "server.h"

gint mx_delete_chat_prepare(cJSON *root,
                            sqlite3_stmt **stmt,
                            gint sender_id) {
    gchar *query = "DELETE FROM messages WHERE chat_id = ?;";
    gint receiver_id = cJSON_GetObjectItem(root, "user_id")->valueint;
    gint64 chat_id = mx_get_chat_id(sender_id, receiver_id);
    sqlite3 *db = *(mx_get_db());
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_delete_chat_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_int64(*stmt, 1, chat_id)) != SQLITE_OK)
        g_warning("mx_delete_chat_prepare bind: chat_id:%lld %d\n",
                  chat_id, rc);
    return rc;
}

gint mx_delete_chat_run(sqlite3_stmt *stmt) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());

    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        g_message("mx_delete_chat_run SUCCESS\n");
    else
        g_warning("mx_delete_chat_run step rc:%d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_delete_chat_run finalize rc:%d\n", rc);
    return rc;
}

static gchar *mx_delete_chat_response(gint user_id) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "Chat successfly deleted";
    gchar *response = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_CLEAR_CHAT));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    cJSON_AddItemToObject(json, "user_id", cJSON_CreateNumber(user_id));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

void mx_delete_chat(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;
    gint receiver_id = cJSON_GetObjectItem(root, "user_id")->valueint;

    // if (is_valid(root) == FALSE) {
        // g_warning("Invalid send_message request\n");
        // return;
    // }
    if ((rc = mx_delete_chat_prepare(root, &stmt, client->uid)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare failed: %d\n", rc);
        // TODO: send error?;
    if ((rc = mx_delete_chat_run(stmt)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_run failed: %d\n", rc);
        // TODO: send error?;
    else {
        gchar *response = mx_delete_chat_response(receiver_id );

        mx_send_data(client->data_out, response);
        g_free(response);
    }
    return;
}
