#ifndef CLIENT_H
#define CLIENT_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "mx_json.h"

enum e_auth_data_validation {
    MX_VALID = 0,
    MX_LOGIN_ERROR,
    MX_PASSWORD_ERROR,
    MX_REPEAT_PASSWORD_ERROR,
};

#define MX_FORBIDEN_PATTERN "[^A-Z&^a-z&^0-9&^(!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~)]"
#define MX_ALLOWED_PATTERN "^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]).{6,48}$"

typedef struct s_contact_data {
    GtkWidget *row;
    gchar *first_name;
    gchar *last_name;
    gint id;
    GtkWidget *popup;
}              t_contact_data;


typedef struct s_client {
    GSocketConnection *connection;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
    gchar *token;

    gint chat_with;

    GtkApplication *app;
    GtkBuilder *builder;
    GtkWindow *phone_entering;
    GtkWindow *password_validation;
    GtkWindow *profile_setuping;
    GtkWindow *main_window;
    GtkWindow *add_contact_dialog;
    GtkWindow *contact_info_window;
    GtkWindow *profile_window;

    GtkWidget *chat_box;
    GtkListBox *contacts;
    GtkListBox *chat;
    GtkWidget *contact_info;
    GtkWidget *contact_view;
    GtkWidget *scroll;

    GtkWidget *text_view;

    GHashTable *contacts_table;
    GtkWidget *label;

    void (*response_handler[30])(cJSON *json, struct s_client *client);
}              t_client;


t_client **mx_get_client(void);
void mx_init_handlers(t_client *client);
void mx_receive_data(gchar *data, t_client *client);
void mx_get_data(GObject *source_object, GAsyncResult *res,
                 gpointer user_data);
gssize mx_send_data(GDataOutputStream *data_out, gchar *data);

// screens
void mx_apply_styles(const gchar *path_to_css);
void mx_window_switcher(GtkWindow *hide, GtkWindow *show);
GtkWindow *mx_phone_entering_window(t_client *client);
GtkWindow *mx_password_validate_window(t_client *client);
GtkWindow *mx_profile_setuping_window(t_client *client);
GtkWindow *mx_main_window(t_client *client);
GtkWindow *mx_add_contact_dialog(t_client *client);
GtkWindow *mx_profile_window(t_client *client);
GtkWindow *mx_contact_info_window(t_client *client);


void mx_chat_control(GtkBuilder *builder, t_client *client);
void mx_top_bar_control(GtkBuilder *builder, t_client *client);
gboolean mx_close_widget(GtkWidget *widget, GdkEventKey *event,
                         GtkWidget *to_close);
void mx_set_entry_value(GtkBuilder *builder, const gchar *entry_name,
                        const gchar *value);
void mx_scroll_to_bottom(t_client *client);
gchar *mx_get_message_from_entry(GtkWidget *text_view);
void mx_show_message_in_ui(t_client *client, gchar *message_text);
void mx_remove_rows(GtkListBox *listbox);
void mx_show_contact_in_ui(t_client *client, gchar *first_name,
                           gchar *last_name, gint user_id);
void get_profile(GtkButton *button, gpointer data);
gboolean mx_menu_callback(GtkWidget *widget, GdkEventButton *event,
                          GtkWidget *menu);
GtkWidget *mx_contact_context(t_contact_data *contact);
GtkWidget *mx_profile_context(t_client *client);

// requests
gchar *mx_password_request(const gchar *phone);
gchar *mx_auth_request(const gchar *phone, const gchar *password);
gchar *mx_sign_up_request(const gchar *phone, const gchar *first_name,
                          const gchar *last_name, const gchar *email);
gchar *mx_find_contact_request(const gchar *phone, const gchar *token);
gchar *mx_add_contact_request(gint user_id, const gchar *token);
gchar *mx_contact_list_request(const gchar *token);
gchar *mx_send_message_request(gint user_id, const gchar *token,
                               const gchar *message);
gchar *mx_chat_history_request(gint user_id, const gchar *token,
                               gint from, gint to);
gchar *mx_sign_out_request(const gchar *token);
gchar *mx_remove_contact_request(gint user_id, const gchar *token);
gchar *mx_profile_data_request(const gchar *token);
gchar *mx_change_data_request(const gchar *first_name,
                              const gchar *last_name, const gchar *email,
                              const gchar *token);
gchar *mx_clear_chat_request(gint user_id, const gchar *token);
gchar *mx_contact_info_request(gint user_id, const gchar *token);

// responses
void mx_auth_validated(cJSON *json, t_client *client);
void mx_contact_list(cJSON *json, t_client *client);
void mx_contact_not_found(cJSON *json, t_client *client);
void mx_get_contact(cJSON *json, t_client *client);
void mx_invalid_password(cJSON *json, t_client *client);
void mx_handle_password(cJSON *json, t_client *client);
void mx_sms_error(cJSON *json, t_client *client);
void mx_sign_up_user(cJSON *json, t_client *client);
void mx_send_message(cJSON *json, t_client *client);
void mx_get_chat_history(cJSON *json, t_client *client);
void mx_remove_contact(cJSON *json, t_client *client);
void mx_add_contact(cJSON *json, t_client *client);
void mx_sign_out(cJSON *json, t_client *client);
void mx_profile_data(cJSON *json, t_client *client);
void mx_clear_chat(cJSON *json, t_client *client);
void mx_contact_info(cJSON *json, t_client *client);
void mx_change_profile(cJSON *json, t_client *client);
void mx_get_message_handler(cJSON *json, t_client *client);


// validation
gint mx_auth_confirming(gchar *login, gchar *password,
                        gchar *repeat_password);

// Utils
gboolean mx_match(const gchar *str, const gchar *pattern,
                  gint compile_flags, gint match_flag);

int mx_application_run(int argc, char **argv, GtkApplication *app);
void mx_application_init(t_client *client);
void shut_down(GtkApplication *app, t_client *client);

#endif /* end of include guard: CLIENT_H */
