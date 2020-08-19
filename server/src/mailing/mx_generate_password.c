#include "server.h"

void swap(char **pass, int indexI, int indexJ) {
    char temp = (*pass)[indexI];

    (*pass)[indexI] = (*pass)[indexJ];
    (*pass)[indexJ] = temp;
}

char *mx_generate_password(void) {
    int length = 6 + rand() % 2;
    char *pass = malloc(sizeof(char) * length + 1);

    srand((unsigned)time(NULL) / 2);
    for (int i = 0; i < length + 1; i++)
        pass[i] = '\0';
    pass[0] = rand() % 10 + 48;
    pass[1] = rand() % 10 + 48;
    pass[2] = rand() % 10 + 48;
    pass[3] = rand() % (122 - 97) + 97;
    for (int i = 4; i < length; i++)
        pass[i] = rand() % (90 - 65) + 65;
    for (int i = 3; i; i--)
        swap(&pass, rand() % length, rand() % length);
    return pass;
}
