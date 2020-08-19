#include "server.h"

sqlite3 **mx_get_db(void) {
    static sqlite3 *db = NULL;

    return &db;
}
