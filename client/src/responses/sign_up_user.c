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
void mx_sign_up_user(cJSON *json, t_client *client) {
    if (json_validator(json)) {
        mx_window_switcher(client->password_validation, client->profile_setuping);
    }
    else {
        g_message("Invalid sign out response\n");
    }

}
