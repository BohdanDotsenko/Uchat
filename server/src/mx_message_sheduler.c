#include "server.h"

gint mx_message_sheduler_prepare(sqlite3_stmt **stmt,
                                 sqlite3 *db) {
    gint date = g_get_real_time() / 1000000;
    gint rc = 0;

    gchar *query = "SELECT sender_id, receiver_id FROM messages\
                    WHERE delivery_date <= ? AND is_read = 0;";
    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_message_sheduler_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_int(*stmt, 1, date)) != SQLITE_OK)
        g_warning("mx_message_sheduler_prepare bind: date:%d %d\n",
                  date, rc);
    return rc;
}

void mx_send_notification(t_client *client,
                          sqlite3_stmt *stmt) {
    cJSON *notification = cJSON_CreateObject();
    gchar *response = NULL;

    cJSON_AddNumberToObject(notification, "response_type", RS_NEW_MESSAGE);
    cJSON_AddNumberToObject(notification, "sender_id",
                            sqlite3_column_int(stmt, 0));
    cJSON_AddNumberToObject(notification, "receiver_id",
                            sqlite3_column_int(stmt, 1));
    response = cJSON_PrintUnformatted(notification);
    mx_send_data(client->data_out, response);
    cJSON_Delete(notification);

}
void mx_message_sheduler_reset(sqlite3_stmt *stmt) {
    gint rc = 0;

    if ((rc = sqlite3_reset(stmt)) == SQLITE_OK) {
        gint date = g_get_real_time() / 1000000;

        sqlite3_clear_bindings(stmt);
        if ((rc = sqlite3_bind_int(stmt, 1, date)) != SQLITE_OK)
            g_warning("mx_message_sheduler_prepare bind: date:%d %d\n",
                      date, rc);
    }
    else
        g_warning("mx_message_sheduler_reset failed to reset:%d", rc);
}

gpointer mx_message_sheduler(gpointer data) {
    sqlite3 *db = *(mx_get_db());
    sqlite3_stmt *stmt = NULL;
    t_client *client = NULL;
    GHashTable **online_users = mx_get_online_users();

    mx_message_sheduler_prepare(&stmt, db);
    while (TRUE) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            gint receiver_id = sqlite3_column_int(stmt, 1);

            if ((client = g_hash_table_lookup(*online_users,
                                              GINT_TO_POINTER(receiver_id)))) {
                g_message("client->uid = %d", client->uid);
                mx_send_notification(client, stmt);
            }
        }
        g_usleep(5000000);
        mx_message_sheduler_reset(stmt);
    }
    (void)data;
}
