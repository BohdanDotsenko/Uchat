#include <gtk/gtk.h>
#include <gio/gio.h>
#include <string.h>
#define MX_TITLE "uChat"
#define MX_WELCOME "Welcome to the macOS application"

typedef struct temp {
    GSocket *client;
    GSocketAddress *address;
} data;

typedef struct s_form {
    data *data_cl;
    GtkWidget *window;
    GtkWidget *body;
    GtkWidget *header;
    GtkWidget *header_items;
    GtkWidget *form;
    GtkWidget *inputs;
    GtkWidget *floor;
    GtkWidget *login_input;
    GtkWidget *password_input;
    GtkWidget *image;
    GtkWidget *logo;
    GtkWidget *welcome_label;
    GtkWidget *log_in_btn;
    GtkCssProvider *provider;
}               t_form;

void validation(GtkWidget *button, gpointer data) {
    const gchar *login = gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input));
    gint size = strlen(login);
    printf("validation\n");
    printf("login = %s\n", gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input)));
    printf("password = %s\n", gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->password_input)));
    g_socket_send_to(((t_form *)data)->data_cl->client, ((t_form *)data)->data_cl->address, login, size, NULL, NULL);

    // g_data_output_stream_put_string(((t_form *)data)->data_cl->output, gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input)), NULL, NULL);
    // g_data_output_stream_put_string(((t_form *)data)->data_cl->output, gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->password_input)), NULL, NULL);
    // exit(1);
}


void activate(GtkApplication* app, gpointer user_data) {
    t_form *form_template = (t_form *)malloc(sizeof(t_form));
    int win_xsize;
    int win_ysize;
    form_template->data_cl = (data *)user_data;

    form_template->provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path (form_template->provider, "./theme.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                           GTK_STYLE_PROVIDER(form_template->provider),
                           GTK_STYLE_PROVIDER_PRIORITY_USER);


    form_template->window = gtk_application_window_new(app);
    form_template->body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    form_template->header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    form_template->header_items = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    form_template->form = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    form_template->inputs = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    form_template->floor = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    form_template->image = gtk_image_new_from_file("email.png");
    form_template->logo = gtk_label_new(MX_TITLE);
    form_template->welcome_label = gtk_label_new(MX_WELCOME);
    form_template->login_input = gtk_entry_new();
    form_template->password_input = gtk_entry_new();
    form_template->log_in_btn = gtk_button_new_with_label("Log in");

    gtk_window_get_size(GTK_WINDOW(form_template->window), &win_xsize, &win_ysize);

    gtk_widget_set_name(form_template->image, "image");
    gtk_widget_set_name(form_template->logo, "logo");
    gtk_widget_set_name(form_template->welcome_label, "welcome_label");
    gtk_widget_set_name(form_template->login_input, "login_input");
    gtk_widget_set_name(form_template->password_input, "password_input");
    gtk_widget_set_name(form_template->log_in_btn, "log_in");


    gtk_window_set_title(GTK_WINDOW(form_template->window), MX_TITLE);
    gtk_window_set_position(GTK_WINDOW(form_template->window), GTK_WIN_POS_CENTER);
    // gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(form_template->window), 0.2 * win_xsize);

    gtk_container_add(GTK_CONTAINER(form_template->window), form_template->body);
    gtk_widget_set_halign(form_template->body, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(form_template->body, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->header);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->form);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->floor);

    gtk_box_pack_start(GTK_BOX(form_template->header), GTK_WIDGET(form_template->header_items), TRUE, FALSE, 0.1 * win_xsize);
    gtk_box_pack_start(GTK_BOX(form_template->header_items), GTK_WIDGET(form_template->image), TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(form_template->header_items), GTK_WIDGET(form_template->logo), FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(form_template->header_items), GTK_WIDGET(form_template->welcome_label), FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(form_template->form), GTK_WIDGET(form_template->inputs), TRUE, FALSE, 0.5 * win_ysize);
    gtk_box_pack_start(GTK_BOX(form_template->inputs), GTK_WIDGET(form_template->login_input), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_template->inputs), GTK_WIDGET(form_template->password_input), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_template->floor), GTK_WIDGET(form_template->log_in_btn), TRUE, TRUE, 0.85 * win_xsize);

    gtk_entry_set_activates_default(GTK_ENTRY(form_template->login_input), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(form_template->login_input), "Email address");
    gtk_entry_set_placeholder_text(GTK_ENTRY(form_template->password_input), "Password");
    gtk_entry_set_max_length(GTK_ENTRY(form_template->password_input), 72);
    gtk_entry_set_visibility(GTK_ENTRY(form_template->password_input), FALSE);

    g_signal_connect(G_OBJECT(form_template->log_in_btn), "clicked", G_CALLBACK(validation), form_template);

    g_signal_connect(form_template->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(form_template->window);
}

void activate_chat(GtkApplication* app, gpointer user_data) {
    GtkBuilder *builder = NULL;
    GtkWidget *window = NULL;
    builder = gtk_builder_new ();
    // window = gtk_application_window_new(app);
    gtk_builder_add_from_file(builder, "./gui.glade", NULL);
    // window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    // gtk_builder_connect_signals(builder, NULL);

    gtk_builder_connect_signals(builder, builder);
    // g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
}

void entry_smth(GtkApplication* app, gpointer user_data) {
    t_form *form_template = (t_form *)malloc(sizeof(t_form));

    form_template->window = gtk_application_window_new(app);
    form_template->login_input = gtk_entry_new();
    form_template->log_in_btn = gtk_button_new_with_label("Push");

    gtk_container_add(GTK_CONTAINER(form_template->window), form_template->login_input);

    g_signal_connect(G_OBJECT(form_template->log_in_btn), "clicked", G_CALLBACK(validation), form_template);

    g_signal_connect(form_template->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(form_template->window);
}

int login(int argc, char **argv, gpointer user_data) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gnome.clocks.desktop", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), user_data);
  status = g_application_run (G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
