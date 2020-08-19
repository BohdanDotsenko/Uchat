#include "server.h"

#define MX_TEMPLATE_SIZE 10000
#define MX_MESSAGE_SIZE 100

#define MX_FROM_NUMBER "+12019285337"
#define MX_MESSAGE "Your Uchat verification code is: "

gchar *mx_create_sms_body(gchar *to_number, gchar *password) {
    gchar parameters[MX_TEMPLATE_SIZE];
    gchar message[MX_MESSAGE_SIZE];
    gchar *body = NULL;

    g_snprintf(message, sizeof(message), "%s%s", MX_MESSAGE, password);
    g_snprintf(parameters, sizeof(parameters), "%s%s%s%s%s%s", "To=",
               to_number, "&From=", MX_FROM_NUMBER, "&Body=", message);
    body = g_strdup(parameters);
    g_print("body = %s\n", body);
    return body;
}
