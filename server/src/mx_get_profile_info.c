#include "server.h"

gint mx_get_profile_info_prepare(cJSON *root,
                              sqlite3_stmt **stmt,
                              gint sender_id) {
    sqlite3 *db = *(mx_get_db());
    gint rc = 0;
    gchar *query = "SELECT first_name, last_name, email from user_profile\
                    WHERE user_id = ?;";

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_get_profile_info_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_int(*stmt, 1, sender_id)) != SQLITE_OK)
        g_warning("mx_get_profile_info_prepare bind: uid:%d %d\n",
                  sender_id, rc);
    (void)root;
    return rc;
}

gchar *mx_get_profile_info_response(sqlite3_stmt *stmt) {
    cJSON *profile_info = cJSON_CreateObject();
    gchar *response = NULL;

    cJSON_AddNumberToObject(profile_info, "response_type",
                            RS_PROFILE_DATA);
    cJSON_AddStringToObject(profile_info, "first_name",
                            (gchar*)sqlite3_column_text(stmt, 0));
    cJSON_AddStringToObject(profile_info, "last_name",
                            (gchar*)sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(profile_info, "email",
                            (gchar*)sqlite3_column_text(stmt, 2));
    response = cJSON_PrintUnformatted(profile_info);
    cJSON_Delete(profile_info);
    return response;
}

gint mx_get_profile_info_run(sqlite3_stmt *stmt) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        return rc;
    }
    else if (rc == SQLITE_DONE)
        g_warning("mx_get_profile_info_run step: no message for chat_id\n");
    else
        g_warning("mx_get_profile_info_run step rc:%d, %s\n",
                  rc, sqlite3_errmsg(db));
    return rc;
}

void mx_get_profile_info(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;

    // if (is_valid(root) == FALSE) {
        // g_warning("Invalid send_message request\n");
        // return;
    // }
    if ((rc = mx_get_profile_info_prepare(root, &stmt, client->uid)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare failed: %d\n", rc);
        // TODO: send error?;
    if ((rc = mx_get_profile_info_run(stmt)) != SQLITE_ROW)
        g_warning("mx_get_profile_info_run failed: %d\n", rc);
        // TODO: send error?;
    else {
        gchar *response = mx_get_profile_info_response(stmt);

        mx_send_data(client->data_out, response);
        g_free(response);
    }
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_change_profile_run finalize rc:%d\n", rc);
    return;
}
