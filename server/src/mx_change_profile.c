#include "server.h"

gint mx_change_profile_prepare(cJSON *root,
                              sqlite3_stmt **stmt,
                              gint sender_id) {
    sqlite3 *db = *(mx_get_db());
    gchar *query = "UPDATE user_profile SET first_name = ?, last_name = ?,\
                    email = ? WHERE user_id = ?;";
    gchar *first_name = cJSON_GetObjectItem(root, "first_name")->valuestring;
    gchar *last_name = cJSON_GetObjectItem(root, "last_name")->valuestring;
    gchar *email = cJSON_GetObjectItem(root, "email")->valuestring;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_change_profile_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_text(*stmt, 1, first_name, -1, NULL)) != SQLITE_OK)
        g_warning("mx_change_profile_prepare bind: first_name:%s %d\n",
                  first_name, rc);
    if ((rc = sqlite3_bind_text(*stmt, 2, last_name, -1, NULL)) != SQLITE_OK)
        g_warning("mx_change_profile_prepare bind: last_name:%s %d\n",
                  last_name, rc);
    if ((rc = sqlite3_bind_text(*stmt, 3, email, -1, NULL)) != SQLITE_OK)
        g_warning("mx_change_profile_prepare bind: email:%s %d\n",
                  email, rc);
    if ((rc = sqlite3_bind_int(*stmt, 4, sender_id)) != SQLITE_OK)
        g_warning("mx_change_profile_prepare bind: uid:%d %d\n",
                  sender_id, rc);
    return rc;
}

gint mx_change_profile_run(sqlite3_stmt *stmt) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());

    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        g_message("mx_change_profile_run SUCCESS\n");
    else
        g_warning("mx_change_profile_run step rc:%d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_change_profile_run finalize rc:%d\n", rc);
    return rc;
}

static gchar *mx_message_send_response(void) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "Profile successfuly updated";
    gchar *response = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_CHANGE_PROFILE));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

void mx_change_profile(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;

    // if (is_valid(root) == FALSE) {
        // g_warning("Invalid send_message request\n");
        // return;
    // }
    if ((rc = mx_change_profile_prepare(root, &stmt, client->uid)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare failed: %d\n", rc);
        // TODO: send error?;
    if ((rc = mx_change_profile_run(stmt)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_run failed: %d\n", rc);
        // TODO: send error?;
    else {
        gchar *response = mx_message_send_response();

        mx_send_data(client->data_out, response);
        g_free(response);
    }
    return;
}
