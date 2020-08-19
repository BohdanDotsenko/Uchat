#include "server.h"

static gboolean find_socket(gpointer key,
                            gpointer value,
                            gpointer user_data) {
    if (*(gint64 *)key == *(gint64 *)user_data) { // ASK : maybe change on pointers equals
        return TRUE;
    }
    return FALSE;
    (void)value;
}

GDataOutputStream *mx_get_socket_by_user_id(gint64 user_id) {
    GHashTable **online_users = mx_get_online_users();
    gpointer socket = g_hash_table_find(*online_users, find_socket, &user_id);

    return socket ? ((t_client *)socket)->data_out : NULL;
}
