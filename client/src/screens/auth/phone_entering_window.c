#include "client.h"

#define MX_PHONE_INPUT_WINDOW "client/src/screens/glade/phone_entering_window.glade"
#define MX_STYLES "client/src/screens/auth/auth.css"
#define MX_ALLOW_PATTERN "[^+^0-9]"

static gboolean phone_validator(GtkBuilder *builder, GtkEntryBuffer *buffer) {
    GtkLabel *info = GTK_LABEL(gtk_builder_get_object(builder,
                                                      "phone_info_mess"));
    const gchar *phone = gtk_entry_buffer_get_text(buffer);
    gchar *message = NULL;

    if (strlen(phone) != 13) {
        message = "Phone must includes county code (like, +38)\n"
                  "and than 10 numbers of your number";
        gtk_label_set_text(info, message);
        gtk_widget_show(GTK_WIDGET(info));
        return FALSE;
    }
    if (mx_match(phone, MX_ALLOW_PATTERN, 0, 0)) { // TODO: change regex settings
        message = "The phone includes forbidden character";
        gtk_label_set_text(info, message);
        gtk_widget_show(GTK_WIDGET(info));
        return FALSE;
    }
    return TRUE;
}

static void generate_pass(GtkWidget *widget, t_client *client) {
    GtkEntryBuffer *buffer = NULL;
    GtkBuilder *builder = client->builder;
    GtkEntry *phone = GTK_ENTRY(gtk_builder_get_object(builder, "phone"));
    const gchar *phone_value = gtk_entry_get_text(phone);
    gchar *request = mx_password_request(phone_value);

    gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder,
                                                      "phone_info_mess")));
    buffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "phone_buff"));
    gtk_entry_buffer_set_text(buffer, phone_value, -1);
    if (phone_validator(builder, buffer))
        mx_send_data(client->data_out, request);
    gtk_entry_set_text(phone, "");
    g_free(request);
    return;
    (void)widget;
}

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *next = NULL;
    GtkEntry *phone = NULL;
    GtkEntryBuffer *buffer = NULL;

    buffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "phone_buff"));
    next = GTK_BUTTON(gtk_builder_get_object(builder, "first_next_button"));
    phone = GTK_ENTRY(gtk_builder_get_object(builder, "phone"));
    gtk_entry_set_text(phone, gtk_entry_buffer_get_text(buffer));
    g_signal_connect(phone, "activate", G_CALLBACK(generate_pass), client);
    g_signal_connect(next, "clicked", G_CALLBACK(generate_pass), client);
}



GtkWindow *mx_phone_entering_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_PHONE_INPUT_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(shut_down), client);
    controling(builder, client);
    return window;
}
