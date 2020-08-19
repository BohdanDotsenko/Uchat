#include "server.h"

static gboolean json_validator(cJSON *root, t_client *client) {
    cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");
    cJSON *user_id = cJSON_GetObjectItemCaseSensitive(root, "user_id");

    if (token && cJSON_IsString(token)
        && user_id && cJSON_IsNumber(user_id)) {
        if (!g_strcmp0(token->valuestring, client->token)) {
            return TRUE;
        }
        else
            return FALSE;
    }
    else
        return FALSE;
}


static gchar *mx_remove_contact_run(sqlite3_stmt *stmt, gint contact) {
    sqlite3 *db = *(mx_get_db());
    gint rc = 0;
    cJSON *json = cJSON_CreateObject();
    gchar *response = NULL;

    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE) {
        cJSON_AddNumberToObject(json, "response_type", RS_REMOVE_CONTACT);
        cJSON_AddNumberToObject(json, "contact", contact);
        cJSON_AddStringToObject(json, "message", "Contact remove successfully");
        response = cJSON_PrintUnformatted(json);
        if (!response) {
            g_warning("Failed to print mx_add_contact_response.\n");
        }
        cJSON_Delete(json);
    }
    else
        g_warning("mx_add_contact_run step rc:%d, %s\n", rc, sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_add_contact_run finalize rc:%d\n", rc);
    return response;
}

static gint mx_remove_contact_prepare(sqlite3_stmt **stmt,
                                      gint contact,
                                      gint user_id) {
    sqlite3 *db = *(mx_get_db());
    gchar *query = "DELETE FROM user_contact_list WHERE\
                    user_id = ? AND contact = ?;";
    gint rc = 0;
    g_message("uid:%d, cont:%d", user_id, contact);

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_remove_contact_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_int(*stmt, 1, user_id)) != SQLITE_OK)
        g_warning("mx_remove_contact_prepare bind: user_id:%d %d\n",
                  contact, rc);
    if ((rc = sqlite3_bind_int(*stmt, 2, contact)) != SQLITE_OK)
        g_warning("mx_remove_contact_prepare bind: contact:%d %d\n",
                  contact, rc);
    return rc;
}


void mx_remove_contact_handler(cJSON *root, t_client *client) {
    if (json_validator(root, client)) {
        gint contact = cJSON_GetObjectItem(root, "user_id")->valueint;
        sqlite3_stmt *stmt= NULL;
        gchar *response = NULL;

        mx_remove_contact_prepare(&stmt, contact, client->uid);
        response = mx_remove_contact_run(stmt, contact);
        if (response) {
            mx_send_data(client->data_out, response);
            g_free(response);
        }
        return;
    }
    g_warning("Not valid request\n");
}
