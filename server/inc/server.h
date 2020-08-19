#ifndef SERVER_H
#define SERVER_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "mx_json.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <stdlib.h>
#include "curl/curl.h"
#include <time.h>
#include <ldap.h>
#include "glib/gprintf.h"
#include <sys/stat.h>

#define MESSAGE_DELAY_RANGE_BEGIN 5
#define MESSAGE_DELAY_RANGE_END 8

typedef struct s_client {
    GSocketConnection *connection;
    GInputStream *istream;
    GOutputStream *ostream;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;

    gint32 uid;
    gchar *token;
    gchar *password;
    void (*request_handler[15])();
}              t_client;

/*
   contain connection from new client and hash table in which should be add
   uid as a key and streams of client as a value
*/
typedef struct s_new_client {
    GHashTable *online_users;
    t_client *client;
}              t_new_client;


void mx_daemon(void);
sqlite3 **mx_get_db(void);
void mx_receiver(gchar *data, t_client *client);
GHashTable **mx_get_online_users(void);
sqlite3 **mx_get_db(void);

GDataOutputStream *mx_get_socket_by_user_id(gint64 user_id);
gssize mx_send_data(GDataOutputStream *data_out, gchar *data);

//database
void mx_db_init(void);

//Sign_up_rq
// gint mx_sign_up_quary(cJSON *root, sqlite3 *db);
gboolean mx_check_user_excist(const gchar *phone, sqlite3 *db);
void mx_sign_up(cJSON *root, t_client *client);

//Sign_up_rq
gint get_user_id_run(sqlite3_stmt *stmt, t_client *client);
gint get_user_id_prepare(cJSON *root, sqlite3_stmt **stmt);
void mx_sign_in(cJSON *root, t_client *client);

// Messages requests
gint64 mx_get_chat_id(gint32 uid1, gint32 uid2);
void mx_recovery_password(cJSON *root, t_client *client);
char *mx_generate_password(void);

gint mx_send_sms(gchar *body);

gchar *mx_create_sms_body(gchar *to_number, gchar *password);
char *mx_recovery_body(gchar *user_name, gchar *password);
gchar *mx_notify_body(gchar *user_name);


void mx_password_request_handler(cJSON *root, t_client *client);
void mx_auth_request_handler(cJSON *root, t_client *client);
void mx_sign_up_request_handler(cJSON *root, t_client *client);
void mx_get_contact_handler(cJSON *root, t_client *client);
void mx_add_contact_handler(cJSON *root, t_client *client);
void mx_sign_out_request_handler(cJSON *root, t_client *client);
void mx_remove_contact_handler(cJSON *root, t_client *client);
void mx_setup_as_read(cJSON *root, t_client *client);
void mx_get_contact_info(cJSON *root, t_client *client);

void mx_add_user_to_bd(cJSON *root, t_client *client, sqlite3 *db);
gint mx_get_user_id_by_phone(gchar *phone, sqlite3 *db);
gchar *mx_auth_send_response(t_client *client, gchar *phone);

gchar *mx_create_token(gchar *login, gchar *pass);
gchar *mx_send_error_response(gint type, gchar *message);

void mx_send_message(cJSON *root, t_client *client);
void mx_get_chat_history(cJSON *root, t_client *client);
void mx_get_contacts_list(cJSON *root, t_client *client);
gint mx_get_contact_handler_prepare(sqlite3_stmt **stmt,
                                    gint user_id,
                                    sqlite3 *db);
// cJSON *mx_get_contact_handler_response(sqlite3_stmt *stmt, gint user_id);
gpointer mx_message_sheduler(gpointer data);
cJSON *mx_get_contact_handler_run(sqlite3_stmt *stmt, gint user_id);
void mx_delete_chat(cJSON *root, t_client *client);
void mx_change_profile(cJSON *root, t_client *client);
void mx_get_profile_info(cJSON *root, t_client *client);
#endif /* end of include guard: SERVER_H */
