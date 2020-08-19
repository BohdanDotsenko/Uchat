#include "client.h"

static gboolean json_validator(cJSON *json) {
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (message && cJSON_IsString(message))
            return TRUE;
    else
        return FALSE;
}

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_handle_password(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;
        GtkBuilder *builder = client->builder;
        GtkLabel *info = GTK_LABEL(gtk_builder_get_object(builder,
                                                          "pass_info_mess"));

        mx_window_switcher(client->phone_entering, client->password_validation);
        gtk_label_set_text(info, message);
        gtk_widget_show(GTK_WIDGET(info));
    }
    else {
        g_message("Invalid password validate response\n");
    }
}
