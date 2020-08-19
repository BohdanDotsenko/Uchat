
SERVER = uchat_server

EMPTY =
SPACE = $(EMPTY) $(EMPTY)
VPATH = VPATH = $(subst $(SPACE), :, $(SRCD))

# server working directory
SERVER_WD = server/

GLIB_H = `pkg-config --cflags glib-2.0 gio-2.0 gtk+-3.0`
cJSON_H = -I ../json/inc
OPENSSL_H = -I /usr/local/opt/openssl/include
CURL_H =

CURL_L = -lcurl
GLIB_L = `pkg-config --cflags --libs glib-2.0  --libs gio-2.0 --libs gtk+-3.0`
DB_L = -lsqlite3

OBJD = obj
SRCD = src src/mailing src/requests ../json/src
INCS = inc
SRCS = $(foreach dir, $(SRCD), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))

#compilier
CC = clang

CFLAGS = -std=c11 $(addprefix -W, all error extra pedantic)

all: $(SERVER)

$(SERVER): $(OBJS)
	@$(CC) -o $@ $^ $(CURL_L) $(GLIB_L) $(DB_L)
	@printf "\033[32;1m$@ created\033[0m\n"

$(OBJS): obj/%.o: %.c $(INCS) | $(OBJD)
	@$(CC) $(CFLAGS) -I $(INCS) $(GLIB_H) $(cJSON_H) $(CURL_H) $(OPENSSL_H) -c $< -o $@
	@printf "\033[32mcompiled: \033[0m$(notdir $<)\n"

$(OBJD):
	@mkdir -p $@
	@printf "\033[32;1m$@ created\033[0m\n"

uninstall: clean
	@rm -rf $(SERVER)
	@printf "\033[34;1mdeleted $(SERVER)\033[0m\n"

clean:
	@rm -rf $(OBJD)
	@rm -rf $(SERVER_WD)/uchat.db
	@printf "\033[34;1mdeleted $(OBJD)\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
