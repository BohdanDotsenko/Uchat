#include "server.h"

gchar *mx_notify_body(gchar *user_name) {
    char *body = (char *)malloc(sizeof(char) * 2000);

    sprintf(body, "<html><body>\r\n<p><strong>%s,</p>\r\n \
            <p>Recentrly someone has sent you a new letter.</p> \
            <p>Please, don't forget to go to application and read it.</p> \
            <p>Have nice time!</p></body></html>\r\n",
            user_name);
    return body;
}
