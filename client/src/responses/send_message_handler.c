#include "client.h"

static gboolean json_validator(cJSON *json) {
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (message && cJSON_IsString(message))
            return TRUE;
    else
        return FALSE;
}

void mx_send_message(cJSON *json, t_client *client) {

    if (json_validator(json)) {
        GtkTextBuffer *buffer = NULL;

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(client->text_view));
        gtk_text_buffer_set_text(buffer, "\0", -1);
    }
    else {
        g_message("Invalid send message response\n");
    }
}
