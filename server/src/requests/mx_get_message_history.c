#include "server.h"

gint mx_get_chat_history_prepare(cJSON *root,
                                 sqlite3_stmt **stmt,
                                 gint sender_id) {
    if (sender_id == 0) {
        g_message("Please login\n");
        return -1;
    }
    sqlite3 *db = *(mx_get_db());
    gchar *query = "SELECT message, sender_id, receiver_id, date FROM messages\
                    WHERE chat_id = ? AND receiver_id = ?\
                    ORDER BY delivery_date ASC LIMIT ?, ?;";
    gint receiver_id = cJSON_GetObjectItem(root, "contact_id")->valueint;
    gint from = cJSON_GetObjectItem(root, "from")->valueint;
    gint to = cJSON_GetObjectItem(root, "to")->valueint;
    gint64 chat_id = mx_get_chat_id(sender_id, receiver_id);
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare prepare: %d", rc);
    if ((rc = sqlite3_bind_int64(*stmt, 1, chat_id)) != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare  bind: chat_id:%d %d",
                  receiver_id, rc);
    if ((rc = sqlite3_bind_int(*stmt, 2, sender_id)) != SQLITE_OK)
      g_warning("mx_get_chat_history_prepare  bind: sender_id:%d %d\n",
                receiver_id, rc);
    if ((rc = sqlite3_bind_int(*stmt, 3, from)) != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare  bind: from:%d %d\n",
                  from, rc);
    if ((rc = sqlite3_bind_int(*stmt, 4, to)) != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare  bind: to:%d %d\n",
                  to, rc);
    return rc;
}

gchar *mx_form_chat_history_response(sqlite3_stmt *stmt) {
    cJSON *chat_history = cJSON_CreateObject();
    cJSON *message_arr = cJSON_CreateArray();
    gchar *response = NULL;
    gint rc = 0;

    while (rc != SQLITE_DONE) {
        cJSON *message = cJSON_CreateObject();
        cJSON_AddStringToObject(message, "message",
                                (gchar*)sqlite3_column_text(stmt, 0));
        cJSON_AddNumberToObject(message, "sender_id",
                                sqlite3_column_int(stmt, 1));
        cJSON_AddNumberToObject(message, "receiver_id",
                                sqlite3_column_int(stmt, 2));
        cJSON_AddNumberToObject(message, "date",
                                sqlite3_column_int(stmt, 3));
        cJSON_AddItemToArray(message_arr, message);
        rc = sqlite3_step(stmt);
    }
    cJSON_AddNumberToObject(chat_history, "response_type",
                            RS_GET_CHAT_HISTORY);
    cJSON_AddItemToObject(chat_history, "messages", message_arr);
    response = cJSON_PrintUnformatted(chat_history);
    cJSON_Delete(chat_history);
    return response;
}

gchar *mx_get_chat_history_run(sqlite3_stmt *stmt) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());
    gchar *response = NULL;

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        response = mx_form_chat_history_response(stmt);
    }
    else if (rc == SQLITE_DONE)
        g_warning("mx_get_chat_history_run step: no message for chat_id\n");
    else
        g_warning("mx_put_chat_in_db_run step rc:%d, %s\n",
                  rc, sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_get_chat_history_run finalize rc:%d\n", rc);
    g_message("%s", response);
    return response;
}

static gboolean is_valid(cJSON *root) {
    cJSON *receiver_id = cJSON_GetObjectItemCaseSensitive(root, "contact_id");

    if (!cJSON_IsNumber(receiver_id))
        return FALSE;
    return TRUE;
}

void mx_get_chat_history(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;
    gchar *response = NULL;

    if (is_valid(root) == FALSE) {
        g_warning("Invalid get_chat_history request\n");
        return;
    }
    if ((rc = mx_get_chat_history_prepare(root, &stmt, client->uid))
        != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare failed: %d\n", rc);
        // TODO: send error?;
    if ((response = mx_get_chat_history_run(stmt)) == NULL)
        g_warning("mx_get_chat_in_db_run failed or no messages in chat");
        // TODO: send error?;
    else {
        mx_send_data(client->data_out, response);
        mx_setup_as_read(root, client);
    }
    return;
}
