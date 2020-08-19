#include "server.h"

gint mx_get_contacts_list_prepare(sqlite3_stmt **stmt,
                                  gint user_id) {
    if (user_id == 0) {
        g_message("Please login\n");
        return -1;
    }
    sqlite3 *db = *(mx_get_db());
    gchar *query = "SELECT contact FROM user_contact_list WHERE user_id = ?;";
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_get_contacts_list_prepare prepare: %d\n", rc);
    if ((rc = sqlite3_bind_int(*stmt, 1, user_id)) != SQLITE_OK)
        g_warning("mx_get_contacts_list_prepare  bind: user_id:%d %d\n",
                  user_id, rc);
    return rc;
}

static cJSON *get_contact(gint user_id) {
    sqlite3 *db = *(mx_get_db());
    sqlite3_stmt *stmt = NULL;
    cJSON *response = NULL;

    mx_get_contact_handler_prepare(&stmt, user_id, db);
    response = mx_get_contact_handler_run(stmt, user_id);
    return response;
}

gchar *mx_get_contact_list_handler_response(sqlite3_stmt *stmt) {
    cJSON *contact_list = cJSON_CreateObject();
    cJSON *contact_arr = cJSON_CreateArray();
    gchar *response = NULL;
    gint rc = 0;

    while (rc != SQLITE_DONE) {
        cJSON *contact = get_contact((gint)sqlite3_column_int(stmt, 0));
        // g_message("%s\n", contact);

        cJSON_AddItemToArray(contact_arr, contact);
        rc = sqlite3_step(stmt);
    }
    cJSON_AddNumberToObject(contact_list, "response_type",
                            RS_CONTACT_LIST);
    cJSON_AddItemToObject(contact_list, "contacts", contact_arr);
    response = cJSON_PrintUnformatted(contact_list);
    cJSON_Delete(contact_list);
    return response;
}

gchar *mx_get_contacts_list_run(sqlite3_stmt *stmt, gint user_id) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());
    gchar *response = NULL;

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        response = mx_get_contact_list_handler_response(stmt);
    }
    else if (rc == SQLITE_DONE)
            g_warning("mx_get_contacts_list_run step: contact_list empty\
                       user_id:%d\n", user_id);
    else
        g_warning("mx_get_contacts_list_run step rc:%d, %s\n",
                  rc, sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_get_contacts_list_run finalize rc:%d\n", rc);
    return response;
}

void mx_get_contacts_list(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;
    gchar *response = NULL;

    mx_get_contacts_list_prepare(&stmt, client->uid);
    response = mx_get_contacts_list_run(stmt, client->uid);
    mx_send_data(client->data_out, response);
    (void)root;
}
