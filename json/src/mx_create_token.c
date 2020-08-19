#include "mx_json.h"

gchar *mx_create_token(gchar *login, gchar *pass) {
    gchar *token = NULL;
    gchar *md5 = g_compute_checksum_for_string(G_CHECKSUM_MD5, login,
                                               strlen(login));
    gchar *data = g_strjoin("", md5, pass, NULL);

    token = g_compute_checksum_for_string(G_CHECKSUM_SHA256,
                                          data, strlen(data));
    g_free(md5);
    g_free(data);
    return token;
}
