#include "server.h"

char *mx_recovery_body(gchar *user_name, gchar *password) {
    char *body = (char *)malloc(sizeof(char) * 2000);

    sprintf(body, "<html><body>\r\n<p><strong>%s,</p>\r\n \
            <p>Someone requested that the password for your Uchat account be \
            generated.</p><p>Please, use this temporary password to enter \
            into your account.</p> \
            <p>Password: <code><strong>%s</strong></code>.</p> \
            <p><em>This password is good until today at midnight and then \
            You must generate a new one.</em></p></body></html>\r\n",
            user_name, password);
    return body;
}
