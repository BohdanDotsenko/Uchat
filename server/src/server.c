#include "server.h"

void mx_dest_client(gpointer data) {
    t_client *socket = (t_client*)data;

    g_object_unref(socket->istream);
    g_object_unref(socket->ostream);
    g_object_unref(socket->data_in);
    g_object_unref(socket->data_out);
    g_object_unref(socket->connection);
    g_free(socket);
}

gint mx_free_client(t_client *client) {
    GHashTable *online_users = *(mx_get_online_users());

    if (client->uid == 0) {
        mx_dest_client(client);
        return 1;
    }
    if (g_hash_table_remove(online_users,
                            GINT_TO_POINTER(client->uid)) == TRUE) {
        mx_dest_client(client);
    }
    else {
        g_warning("Failed to delete user from hash table!");
        return -1;
    }
    return 1;
}



void get_data(GObject *source_object, GAsyncResult *res, gpointer socket) {
    t_client *new_client = (t_client*)socket;
    GError *error = NULL;
    gchar *data = NULL;

    data = g_data_input_stream_read_line_finish(new_client->data_in,
                                                res, NULL, &error);
    if (data)
        mx_receiver(data, new_client);
    else if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    else {
        g_print("Client logout!\n");
        mx_free_client(new_client);
        return;
    }
    g_data_input_stream_read_line_async(new_client->data_in, G_PRIORITY_DEFAULT, NULL, get_data, new_client);
    (void)source_object;
}

/* this function will get called everytime a client attempts to connect */
gboolean incoming_callback(GSocketService *service,
                           GSocketConnection *connection,
                           GObject *source_object,
                           gpointer user_data) {
    GInputStream *istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GOutputStream *ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    GDataInputStream *data_in = g_data_input_stream_new(istream);
    GDataOutputStream *data_out = g_data_output_stream_new(ostream);
    t_client *socket = g_malloc(sizeof(t_client));

    socket->istream = g_object_ref(istream);
    socket->ostream = g_object_ref(ostream);
    socket->data_in = g_object_ref(data_in);
    socket->data_out = g_object_ref(data_out);
    socket->connection = g_object_ref(connection);
    socket->uid = 0;
    g_data_input_stream_read_line_async(socket->data_in, G_PRIORITY_DEFAULT, NULL, get_data, socket);
    (void)service;
    (void)source_object;
    (void)user_data;
    return FALSE;
}

static gboolean is_valid_args(gint argc) {
    if (argc != 2) {
        g_printerr("Usage ./uchat_server <port>\n");
        return FALSE;
    }
    return TRUE;
}

int main(int argc, char **argv) {
    GSocketService *service = NULL;
    GMainLoop *loop = NULL;
    GError *error = NULL;
    GHashTable **online_users = mx_get_online_users();
    gint64 port = -1;

    if (!is_valid_args(argc))
        return 1;
    mx_daemon();
    *online_users = g_hash_table_new(NULL, NULL);
    service = g_socket_service_new();
    mx_db_init();
    port = g_ascii_strtoll(argv[1], NULL, 10);
    if (!g_socket_listener_add_inet_port((GSocketListener*)service, port,
                                         NULL, NULL)) {
        g_error("Invalid port\n");
        g_clear_error(&error);
        return 1;
    }
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), NULL);
    g_thread_new("message sheduler", mx_message_sheduler, *(mx_get_db()));
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    return 0;
}
