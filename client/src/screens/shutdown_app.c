#include "client.h"

void clear_hash_table(gpointer key, gpointer value, gpointer user_data) {
    t_contact_data *contact = (t_contact_data *)value;

    if (contact->first_name)
        g_free(contact->first_name);
    if (contact->last_name)
        g_free(contact->last_name);
    g_free(contact);
    return;
    (void)key;
    (void)user_data;
}

void shut_down(GtkApplication *app, t_client *client) {
    g_object_unref(client->connection);
    g_input_stream_close(G_INPUT_STREAM(client->data_in), NULL, NULL);
    g_object_unref(client->data_in);
    g_output_stream_close(G_OUTPUT_STREAM(client->data_out), NULL, NULL);
    g_object_unref(client->data_out);
    if (client->token)
        g_free(client->token);
    if (client->contacts_table) {
        g_hash_table_foreach(client->contacts_table, clear_hash_table, NULL);
        g_hash_table_destroy(client->contacts_table);
        client->contacts_table = NULL;
    }
    g_application_quit(G_APPLICATION(client->app));
    return;
    (void)app;
}
