#include "server.h"

void mx_setup_as_read(cJSON *root, t_client *client) {
    sqlite3 *db = *(mx_get_db());
    gchar *query = "UPDATE messages SET is_read = 1 WHERE\
                    chat_id = ? AND receiver_id = ? AND is_read = 0;";
    gint contact_id = cJSON_GetObjectItem(root, "contact_id")->valueint;
    g_print("chat_with = %d -> client-uid = %d\n", contact_id, client->uid);
    gint64 chat_id = mx_get_chat_id(client->uid, contact_id);
    gint rc = 0;
    sqlite3_stmt *stmt = NULL;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("mx_setup_as_read_prepare prepare: %d", rc);
    if ((rc = sqlite3_bind_int64(stmt, 1, chat_id)) != SQLITE_OK)
        g_warning("mx_setup_as_read_prepare  bind: chat_id:%lld %d",
                  chat_id, rc);
    if ((rc = sqlite3_bind_int(stmt, 2, client->uid)) != SQLITE_OK)
      g_warning("mx_setup_as_read_prepare  bind: sender_id:%d %d\n",
                contact_id, rc);
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        g_message("Message read!\n");
    }
    else if (rc == SQLITE_DONE)
        g_warning("mx_setup_as_read_run step: no message for chat_id\n");
    else
        g_warning("mx_setup_as_rea_in_db_run step rc:%d, %s\n",
                  rc, sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_setup_as_read_run finalize rc:%d\n", rc);
}
