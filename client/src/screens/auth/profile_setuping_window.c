#include "client.h"

#define MX_PROFILE_SETUP_WINDOW "client/src/screens/glade/profile_setup.glade"
#define MX_STYLES "client/src/screens/auth/auth.css"
#define MX_PATTERN "[^-^A-Za-z]"
#define MX_EMAILPATTERN "[^.^@^0-9^a-z]"

static gboolean validate(GtkBuilder *builder, GtkEntry *first_name,
                         GtkEntry *last_name, GtkEntry *email) {
    gchar *message = NULL;
    GtkLabel *info = GTK_LABEL(gtk_builder_get_object(builder,
                                                      "info_label"));

    if (strlen(gtk_entry_get_text(first_name)) < 1 && !message)
        message = "Wrong name";
    if (strlen(gtk_entry_get_text(last_name)) < 1 && !message)
        message = "Wrong last name";
    if (strlen(gtk_entry_get_text(email)) < 1 && !message)
        message = "Wrong email";
    if (mx_match(gtk_entry_get_text(first_name), MX_PATTERN, 0, 0) && !message)
        message = "Name includes forbidden character";
    if (mx_match(gtk_entry_get_text(last_name), MX_PATTERN, 0, 0) && !message)
        message = "Last name includes forbidden character";
    if (mx_match(gtk_entry_get_text(email), MX_EMAILPATTERN, 0, 0) && !message)
        message = "Email includes forbidden character";
    if (message) {
        gtk_label_set_text(info, message);
        gtk_widget_show(GTK_WIDGET(info));
    }
    return message ? FALSE : TRUE;
}

static void add_info(GtkButton *button, t_client *client) {
    GtkEntry *phone = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                       "phone_not_edit"));
    GtkEntry *first_name = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                            "first_name"));
    GtkEntry *last_name = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                           "last_name"));
    GtkEntry *email = GTK_ENTRY(gtk_builder_get_object(client->builder, "email"));

    if (validate(client->builder, first_name, last_name, email)) {
        gchar *request = mx_sign_up_request(
                        gtk_entry_get_text(phone),
                        gtk_entry_get_text(first_name),
                        gtk_entry_get_text(last_name),
                        gtk_entry_get_text(email));

        mx_send_data(client->data_out, request);
        g_free(request);
    }
    (void)button;
}

static void move_focus(GtkWidget *widget, GtkWidget *to_focus) {
    gtk_widget_grab_focus(to_focus);
    (void)widget;
}


static void go_back(GtkButton *button, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkEntry *first_name = NULL;
    GtkEntry *last_name = NULL;
    GtkEntry *email = NULL;

    first_name = GTK_ENTRY(gtk_builder_get_object(builder, "first_name"));
    last_name = GTK_ENTRY(gtk_builder_get_object(builder, "last_name"));
    email = GTK_ENTRY(gtk_builder_get_object(builder, "email"));
    gtk_entry_set_text(first_name, "");
    gtk_entry_set_text(last_name, "");
    gtk_entry_set_text(email, "");
    mx_window_switcher(client->profile_setuping, client->phone_entering);
    (void)button;
}

static void enter_handling(GtkBuilder *builder, t_client *client) {
    GtkEntry *first_name = NULL;
    GtkEntry *last_name = NULL;
    GtkEntry *email = NULL;

    first_name = GTK_ENTRY(gtk_builder_get_object(builder, "first_name"));
    last_name = GTK_ENTRY(gtk_builder_get_object(builder, "last_name"));
    email = GTK_ENTRY(gtk_builder_get_object(builder, "email"));
    g_signal_connect(first_name, "activate", G_CALLBACK(move_focus), GTK_WIDGET(last_name));
    g_signal_connect(last_name, "activate", G_CALLBACK(move_focus), GTK_WIDGET(email));
    g_signal_connect(email, "activate", G_CALLBACK(add_info), client);
    gtk_widget_grab_focus(GTK_WIDGET(first_name));
}

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *next = NULL;
    GtkButton *back = NULL;
    GtkButton *photo = NULL;

    back = GTK_BUTTON(gtk_builder_get_object(builder, "third_back_button"));
    next = GTK_BUTTON(gtk_builder_get_object(builder, "sign"));
    photo = GTK_BUTTON(gtk_builder_get_object(builder, "photo"));
    enter_handling(builder, client);
    g_signal_connect(next, "clicked", G_CALLBACK(add_info), client);
    g_signal_connect(back, "clicked", G_CALLBACK(go_back), client);
}

GtkWindow *mx_profile_setuping_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder, MX_PROFILE_SETUP_WINDOW, &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(shut_down), client);
    controling(builder, client);
    return window;
}
