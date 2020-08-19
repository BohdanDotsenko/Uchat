#include "client.h"

t_client *init_client(GSocketConnection *connection) {
    t_client *client = g_new(t_client, 1);
    GInputStream *istream = NULL;
    GOutputStream *ostream = NULL;

    istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));

    client->connection = g_object_ref(connection);
    client->data_in = g_data_input_stream_new(istream);
    client->data_out = g_data_output_stream_new(ostream);
    client->builder = gtk_builder_new();
    client->token = NULL;
    client->contacts_table = NULL;
    mx_application_init(client);
    mx_init_handlers(client);
    g_data_input_stream_read_line_async(client->data_in,
                                        G_PRIORITY_DEFAULT, NULL,
                                        mx_get_data, client);
    return client;
}

int main(int argc, char **argv) {
    GSocketClient *socket = NULL;
    GSocketConnection *connection = NULL;
    GError *error = NULL;
    t_client **client = mx_get_client();

    if (argc != 3) {
        g_error("Usage: ./uchat <IP address> <Port>\n");
        return 1;
    }
    socket = g_socket_client_new();
    connection = g_socket_client_connect_to_host(
               socket, argv[1], g_ascii_strtoll(argv[2], NULL, 10),
               NULL, &error);
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    *client = init_client(connection);
    mx_application_run(0, NULL, (*client)->app);
    return 0;
}
