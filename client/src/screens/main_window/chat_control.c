#include "client.h"

static void send_message(GtkButton *button, GtkWidget *text_view) {
    t_client *client = *mx_get_client();
    gchar *text = g_strchug(mx_get_message_from_entry(text_view));

    if (strlen(text) > 0) {
        gchar *request = mx_send_message_request(client->chat_with,
                                                 client->token, text);

        mx_send_data(client->data_out, request);
        g_free(request);
    }
    mx_scroll_to_bottom(client);
    g_free(text);
    (void)button;
}

static gboolean send_by_enter(GtkWidget *widget, GdkEventKey *event,
                              gpointer data) {
    guint keyval = event->keyval;

    if (keyval == GDK_KEY_Return) {
        t_client *client = *mx_get_client();
        gchar *text = mx_get_message_from_entry(widget);

        if (strlen(text) > 0) {
            gchar *request = mx_send_message_request(client->chat_with,
                                                     client->token, text);

            mx_send_data(client->data_out, request);
            g_free(request);
        }
        mx_scroll_to_bottom(client);
        g_free(text);
        return TRUE;
    }
    return FALSE;
    (void)data;
}

void mx_chat_control(GtkBuilder *builder, t_client *client) {
    GtkWidget *text_view = NULL;
    GtkButton *send_btn = NULL;
    GtkWidget *placeholder = GTK_WIDGET(gtk_label_new("U don`t receive letter"));


    client->chat_with = 0;
    client->scroll = GTK_WIDGET(gtk_builder_get_object(client->builder,
                                                       "chat_scroll"));
    client->text_view = GTK_WIDGET(gtk_builder_get_object(builder,
                                                          "message_entry"));
    client->chat_box = GTK_WIDGET(gtk_builder_get_object(builder, "chat_box"));
    client->chat = GTK_LIST_BOX(gtk_builder_get_object(builder, "message_box"));
    send_btn = GTK_BUTTON(gtk_builder_get_object(builder, "send_btn"));
    text_view = GTK_WIDGET(gtk_builder_get_object(builder, "message_entry"));
    gtk_list_box_set_placeholder(client->chat, placeholder);
    gtk_widget_show(placeholder);
    g_signal_connect(text_view, "key_press_event", G_CALLBACK(send_by_enter),
                     text_view);
    g_signal_connect(send_btn, "clicked", G_CALLBACK(send_message), text_view);
}
