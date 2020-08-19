#include "server.h"

GHashTable **mx_get_online_users(void) {
    static GHashTable *online_users = NULL;

    return &online_users;
}
