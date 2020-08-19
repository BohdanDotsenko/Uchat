#include "client.h"

static void add_contact_btn_callback(GtkButton *button, GtkListBox *box) {
    t_client *client = *mx_get_client();

    gtk_widget_show(GTK_WIDGET(client->add_contact_dialog));
    (void)button;
    (void)box;
}

static gboolean searching(gchar *label, gchar *search) {
    for (int i = 0; label[i] && search[i]; i++) {
        if (label[i] != search[i])
            return FALSE;
    }
    return TRUE;
}

static void local_search(GtkEntry *entry, GtkListBox *box) {
    gchar *search = (gchar *)gtk_entry_get_text(entry);
    GtkListBoxRow *row = NULL;

    for (int i = 0; ; i++) {
        if (!(row = gtk_list_box_get_row_at_index(box, i))) {
            break;
        }
        GtkButton *child = GTK_BUTTON(gtk_bin_get_child(GTK_BIN(row)));
        gchar *label = (gchar *)gtk_button_get_label(child);

        if (!searching(label, search))
            gtk_widget_hide(GTK_WIDGET(row));
        else
            gtk_widget_show(GTK_WIDGET(row));
    }
}

void get_profile(GtkButton *button, gpointer data) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;

    request = mx_profile_data_request(client->token);
    mx_send_data(client->data_out, request);
    gtk_widget_show(GTK_WIDGET(client->profile_window));
    g_free(request);
    (void)button;
    (void)data;
}

static gboolean open_by_left_key(GtkWidget *widget, GdkEventButton *event,
                                 t_contact_data *contact) {
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) {
        get_profile(NULL, NULL);
        return TRUE;
    }
    return FALSE;
    (void)widget;
    (void)contact;
}

void mx_top_bar_control(GtkBuilder *builder, t_client *client) {
    GtkButton *add_contact = NULL;
    GtkButton *profile = NULL;
    GtkEntry *search = NULL;
    GtkWidget *popup = mx_profile_context(client);

    profile = GTK_BUTTON(gtk_builder_get_object(builder, "props"));
    add_contact = GTK_BUTTON(
                gtk_builder_get_object(builder, "add_contact_dialog"));

    search = GTK_ENTRY(gtk_builder_get_object(builder, "local_search"));

    g_signal_connect(search, "activate",
                     G_CALLBACK(local_search), client->contacts);
    g_signal_connect(add_contact, "clicked",
                     G_CALLBACK(add_contact_btn_callback), client->contacts);
    g_signal_connect(profile, "button-press-event",
                     G_CALLBACK(open_by_left_key), NULL);
    g_signal_connect(profile, "button-press-event",
                     G_CALLBACK(mx_menu_callback), popup);
}
