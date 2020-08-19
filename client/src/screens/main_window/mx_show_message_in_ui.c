#include "client.h"

void mx_show_message_in_ui(t_client *client, gchar *message_text) {
    GtkWidget *message = gtk_button_new_with_label(message_text);
    GtkWidget *label = gtk_bin_get_child(GTK_BIN(message));

    gtk_widget_set_name(message, "message");
    gtk_widget_set_halign(GTK_WIDGET(message), GTK_ALIGN_FILL);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(label), 0);
    gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
    gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_FILL);
    g_object_set(label, "margin", 5, NULL);
    g_object_set(message, "margin", 10, NULL);
    gtk_container_add(GTK_CONTAINER(client->chat), message);
    gtk_widget_show_all(GTK_WIDGET(client->chat_box));
}
