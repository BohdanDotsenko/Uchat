#include "server.h"

gint mx_get_user_id_by_phone(gchar *phone, sqlite3 *db) {
    gchar *quary = "SELECT user_id FROM users_credential WHERE phone = ?;";
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;
    gint user_id = 0;

    if ((rc = sqlite3_prepare_v2(db, quary, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("check_if_excist prepare: %d\n", rc);
    if ((sqlite3_bind_text(stmt, 1, phone, -1, NULL)) != SQLITE_OK)
        g_warning("check_if_excist bind: phone:%s %d\n",
                  phone, rc);
    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        user_id = -1;
    else if (rc == SQLITE_ROW)
        user_id = sqlite3_column_int(stmt, 0);
    else
        g_warning("get_user_id_run step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("get_user_id_run finalize rc:%d\n", rc);
    return user_id;
}
