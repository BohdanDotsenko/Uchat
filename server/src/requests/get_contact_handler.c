#include "server.h"

static gboolean json_validator(cJSON *root) {
    cJSON *phone = cJSON_GetObjectItemCaseSensitive(root, "phone");

    if (phone) {
        if (cJSON_IsString(phone))
            return TRUE;
        else
            return FALSE;
    }
    else
        return FALSE;
}

gint mx_get_contact_handler_prepare(sqlite3_stmt **stmt,
                                    gint user_id,
                                    sqlite3 *db) {
    if (user_id == -1) {
        g_message("User not excist\n");
        return -1;
    }
    gchar *query = "SELECT first_name, last_name FROM user_profile\
                    WHERE user_id = ?;";
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_get_contact_handler_prepare prepare: %d\n", rc);
    if ((rc = sqlite3_bind_int64(*stmt, 1, user_id)) != SQLITE_OK)
        g_warning("mx_get_contact_handler_prepare  bind: user_id:%d %d\n",
                  user_id, rc);
    return rc;
}

cJSON *mx_get_contact_handler_response(sqlite3_stmt *stmt, gint user_id) {
    cJSON *user_contact = cJSON_CreateObject();
    // gchar *response = NULL;

    // cJSON_AddNumberToObject(user_contact, "response_type", RS_CONTACT);
    cJSON_AddNumberToObject(user_contact, "user_id", user_id);
    cJSON_AddStringToObject(user_contact, "first_name",
                            (gchar*)sqlite3_column_text(stmt, 0));
    cJSON_AddStringToObject(user_contact, "last_name",
                            (gchar*)sqlite3_column_text(stmt, 1));
    // response = cJSON_PrintUnformatted(user_contact);
    return user_contact;
}

cJSON *mx_get_contact_handler_run(sqlite3_stmt *stmt, gint user_id) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());
    cJSON *response = NULL;

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        response = mx_get_contact_handler_response(stmt, user_id);
    }
    else if (rc == SQLITE_DONE)
            g_warning("mx_get_contact_handler_run step: user_profile empty\
                       user_id:%d\n", user_id);
    else
        g_warning("mx_get_contact_handler_run step rc:%d, %s\n",
                  rc, sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_get_contact_handler_run finalize rc:%d\n", rc);
    return response;
}

void mx_get_contact_handler(cJSON *root, t_client *client) {
    if (json_validator(root)) {
        gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;
        sqlite3 *db = *(mx_get_db());
        sqlite3_stmt *stmt = NULL;
        cJSON *response = NULL;
        gint user_id = mx_get_user_id_by_phone(phone, db);

        mx_get_contact_handler_prepare(&stmt, user_id, db);
        g_message("user_id = %d\n", user_id);
        response = mx_get_contact_handler_run(stmt, user_id);
        if (!response || user_id == client->uid) {
            gchar *error = mx_send_error_response(ER_CONTACT_NOT_FOUND,
                                                  "Contact not found");

            mx_send_data(client->data_out, error);
            g_free(error);
            return ;
        }
        cJSON_AddNumberToObject(response, "response_type", RS_CONTACT);
        mx_send_data(client->data_out, cJSON_PrintUnformatted(response));
        cJSON_Delete(response);
        return;
    }
    g_warning("Not valid request\n");
}
