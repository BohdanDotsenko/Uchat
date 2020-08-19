#include "client.h"

void mx_get_data(GObject *source_object, GAsyncResult *res,
                 gpointer user_data) {
    t_client *client = (t_client *)user_data;
    GError *error = NULL;
    gsize size = 0;
    gchar *data = NULL;

    data = g_data_input_stream_read_line_finish(client->data_in, res, &size, &error);
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    if (data) {
        mx_receive_data(data, client);
    }
    g_data_input_stream_read_line_async(client->data_in,
                                        G_PRIORITY_DEFAULT, NULL,
                                        mx_get_data, client);
    (void)source_object;
}
