#include "server.h"

gboolean mx_check_user_excist(const gchar *phone, sqlite3 *db) {
    gchar *query = "SELECT phone FROM users_credential WHERE phone = ?;";
    sqlite3_stmt *stmt = NULL;
    gboolean result = 0;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("check_if_excist prepare: %s\n", sqlite3_errstr(rc));
    if ((sqlite3_bind_text(stmt, 1, phone, -1, NULL)) != SQLITE_OK)
        g_warning("check_if_excist bind: login:%s %s\n",
                  phone, sqlite3_errstr(rc));
    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        result = false;
    else if (rc == SQLITE_ROW)
        result = true;
    else
        g_warning("check_if_excist step rc:%d\n", rc);
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("check_if_excist finalize rc:%d\n", rc);
    return result;
}
