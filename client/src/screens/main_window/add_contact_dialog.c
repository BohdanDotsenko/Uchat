#include "client.h"

#define MX_STYLES "client/src/screens/main_window/dialog.css"
#define MX_MAIN_WINDOW "client/src/screens/glade/contact_add.glade"
#define MX_ALLOW_PATTERN "/^[+]{1}[0-9]{12}$"

static void close_window(GtkWidget *button, GtkWidget *widget) {
    t_client *client = *mx_get_client();
    GtkBuilder *builder = client->builder;
    GtkEntry *search = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                        "dialog_search"));
    GtkEntryBuffer* buf = gtk_entry_get_buffer(search);
    GtkWidget *info = NULL;

    info = GTK_WIDGET(gtk_builder_get_object(builder, "info_label"));
    gtk_entry_buffer_set_text(buf, "\0", -1 );
    if (client->contact_view) {
        gtk_widget_hide(client->contact_view);
    }
    gtk_widget_hide(info);
    gtk_widget_hide(widget);
    (void)button;
}

static gboolean clear_inputs(GtkWidget *widget, GdkEventKey *event,
                             GtkEntry *search) {
    if (event->keyval == GDK_KEY_Escape) {
        close_window(NULL, widget);
        return TRUE;
    }
    return FALSE;
    (void)search;
}

static void find_user(GtkEntry *entry, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkLabel *info = GTK_LABEL(gtk_builder_get_object(builder, "info_label"));
    const gchar *input = gtk_entry_get_text(entry);
    gchar *request = NULL;

    gtk_widget_hide(GTK_WIDGET(info));
    if (client->contact_view) {
        gtk_widget_hide(client->contact_view);
    }
    if (strlen(input) != 13) {
        gtk_label_set_text(info, "Enter full number");
        gtk_widget_show(GTK_WIDGET(info));
        return;
    }
    if (mx_match(input, MX_ALLOW_PATTERN, 0, 0)) {
        gtk_label_set_text(info, "Not valid phone number");
        gtk_widget_show(GTK_WIDGET(info));
        return;
    }
    request = mx_find_contact_request(input, client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}



GtkWindow *mx_add_contact_dialog(t_client *client) {
    GError *error = NULL;
    GtkWindow *window = NULL;
    GtkEntry *search = NULL;
    GtkWidget *close_btn = NULL;

    client->contact_view = GTK_WIDGET(gtk_button_new());
    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(client->builder, MX_MAIN_WINDOW, &error))
        g_error("%s\n", error->message);

    window = GTK_WINDOW(gtk_builder_get_object(client->builder, "contact_dialog"));
    search = GTK_ENTRY(gtk_builder_get_object(client->builder, "dialog_search"));
    close_btn = GTK_WIDGET(gtk_builder_get_object(client->builder, "close"));
    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(client->builder,
                                                           "add_dialog_box")),
                      client->contact_view);
    gtk_entry_set_text(search, "");
    g_signal_connect(search, "activate", G_CALLBACK(find_user), client);
    gtk_window_set_transient_for(window, client->main_window);
    g_signal_connect(close_btn, "clicked", G_CALLBACK(close_window), window);
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(clear_inputs), search);
    return window;
}
