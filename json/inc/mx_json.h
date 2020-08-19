#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <glib-object.h>
#include "cJSON.h"

enum e_request_types {
    RQ_GENERATE_PASS = 0,
    RQ_AUTH = 1,
    RQ_SIGN_UP = 2,
    RQ_GET_CONTACT = 3,
    RQ_ADD_CONTACT = 4,
    RQ_CONTACT_LIST = 5,
    RQ_SEND_MESSAGE = 6,
    RQ_CHAT_HISTORY = 7,
    RQ_SIGN_OUT = 8,
    RQ_REMOVE_CONTACT = 9,
    RQ_CLEAR_CHAT = 10,
    RQ_PROFILE_DATA = 11,
    RQ_CHANGE_PROFILE = 12,
    RQ_CONTACT_INFO = 13,
};

enum e_response_types {
    RS_PASSWORD_SENT,
    RS_SIGN_UP,
    RS_VALID,
    RS_CONTACT_LIST,
    RS_CONTACT,
    RS_ADD_CONTACT,
    RS_SEND_MESSAGE,
    RS_GET_CHAT_HISTORY,
    RS_SIGN_OUT,
    RS_REMOVE_CONTACT,
    RS_NEW_MESSAGE,
    RS_CLEAR_CHAT,
    RS_CONTACT_INFO,
    RS_PROFILE_DATA,
    RS_CHANGE_PROFILE,
};

enum e_response_errors {
    ER_SENT_PASS = RS_CHANGE_PROFILE + 1,
    ER_PASS,
    ER_CONTACT_NOT_FOUND,
    RS_SIZE,
};

gchar *mx_create_token(gchar *login, gchar *pass);

#endif /* end of include guard: JSON_H */
