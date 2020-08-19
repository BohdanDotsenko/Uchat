#include "server.h"

static gboolean json_validator(cJSON *root) {
    cJSON *user_id = cJSON_GetObjectItemCaseSensitive(root, "user_id");

    if (user_id && cJSON_IsNumber(user_id)) {
        return TRUE;
    }
    else
        return FALSE;
}

gchar *mx_add_contact_response(gint contact) {
    cJSON *json = NULL;
    gchar *response = NULL;
    sqlite3 *db = *(mx_get_db());
    sqlite3_stmt *stmt = NULL;

    mx_get_contact_handler_prepare(&stmt, contact, db);
    json = mx_get_contact_handler_run(stmt, contact);
    cJSON_AddNumberToObject(json, "response_type", RS_ADD_CONTACT);
    cJSON_AddStringToObject(json, "message", "Contact add successfully");
    cJSON_AddNumberToObject(json, "contact", contact);
    response = cJSON_PrintUnformatted(json);
    if (!response) {
        g_warning("Failed to print mx_add_contact_response.\n");
    }
    cJSON_Delete(json);
    return response;
}

gchar *mx_add_contact_run(sqlite3_stmt *stmt, gint contact) {
    sqlite3 *db = *(mx_get_db());
    gint rc = 0;

    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE) {
        return mx_add_contact_response(contact);
    }
    else
        g_warning("mx_add_contact_run step rc:%d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_add_contact_run finalize rc:%d\n", rc);
    return NULL;
}

gint mx_add_contact_prepare(sqlite3_stmt **stmt,
                            gint contact,
                            gint user_id) {
    sqlite3 *db = *(mx_get_db());
    gchar *query = "INSERT INTO user_contact_list(user_id, contact)\
                    VALUES(?, ?);";
    gint rc = 0;
    g_message("uid:%d, cont:%d", user_id, contact);

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_add_contact_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_int(*stmt, 1, user_id)) != SQLITE_OK)
        g_warning("mx_add_contact_prepare bind: user_id:%d %d\n",
                  contact, rc);
    if ((rc = sqlite3_bind_int(*stmt, 2, contact)) != SQLITE_OK)
        g_warning("mx_add_contact_prepare bind: contact:%d %d\n",
                  contact, rc);
    return rc;
}

void mx_add_contact_handler(cJSON *root, t_client *client) {
    if (json_validator(root)) {
        gint contact = cJSON_GetObjectItem(root, "user_id")->valueint;
        sqlite3_stmt *stmt= NULL;
        gchar *response = NULL;

        mx_add_contact_prepare(&stmt, contact, client->uid);
        response = mx_add_contact_run(stmt, contact);
        mx_send_data(client->data_out, response);
        return;
    }
    g_warning("Not valid request\n");
}
