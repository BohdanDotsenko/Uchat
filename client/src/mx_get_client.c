#include "client.h"

t_client **mx_get_client(void) {
    static t_client *client = NULL;

    return &client;
}
