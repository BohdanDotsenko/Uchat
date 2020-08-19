#include "client.h"

int mx_application_run(int argc, char **argv, GtkApplication *app) {
    int status;

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}


static void activate(GtkApplication *app, t_client *client) {
    client->phone_entering = mx_phone_entering_window(client);
    client->password_validation = mx_password_validate_window(client);
    client->profile_setuping = mx_profile_setuping_window(client);
    client->main_window = mx_main_window(client);
    client->add_contact_dialog = mx_add_contact_dialog(client);
    client->profile_window = mx_profile_window(client);
    client->contact_info_window = mx_contact_info_window(client);

    gtk_application_add_window(app, client->phone_entering);
    gtk_application_add_window(app, client->password_validation);
    gtk_application_add_window(app, client->profile_setuping);
    gtk_application_add_window(app, client->main_window);
    gtk_application_add_window(app, client->add_contact_dialog);
    gtk_application_add_window(app, client->profile_window);
    gtk_application_add_window(app, client->contact_info_window);
    gtk_widget_show(GTK_WIDGET(client->phone_entering));
}

void mx_application_init(t_client *client) {
    client->app = gtk_application_new("org.gnome.chat.desktop",
                                    G_APPLICATION_FLAGS_NONE);
    g_signal_connect(client->app, "activate", G_CALLBACK(activate), client);

}
