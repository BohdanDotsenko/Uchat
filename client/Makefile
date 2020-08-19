# name
CLIENT = uchat

EMPTY =
SPACE = $(EMPTY) $(EMPTY)
VPATH = VPATH = $(subst $(SPACE), :, $(SRCD))

# client working directory
CLIENT_WD = client/

OBJD = obj
INCD_CLIENT = -Iinc/ -I../json/inc/

GTK_LIBS = -rdynamic `pkg-config --cflags --libs gtk+-3.0`
GTK_INC = `pkg-config --cflags gtk+-3.0`

SRCD = src src/requests src/responses src/screens src/screens/auth \
	   src/screens/main_window ../json/src

INCD = inc
SRCS = $(foreach dir, $(SRCD), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))
INCS = $(foreach dir, $(INCD), $(wildcard $(dir)/*.h))

OBJTEST = $(OBJS)
#compilier
CC = clang

CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic)
CPPFLAGS = -DMX_CLIENT='"$(CLIENT_WD)"' $(INCD_CLIENT) -D_GNU_SOURCE \
		   $(GTK_INC) $(INCD_LIBS) -I/usr/local/opt/openssl/include
LIBS = $(GTK_LIBS) $(LIBD_LIBS) -L/usr/local/opt/openssl/lib \
	   -lssl -lcrypto -lsqlite3

all: $(CLIENT)

$(CLIENT): $(OBJS)
	@$(CC) -o $@ $^ $(LIBS)
	@printf "\033[32;1m$@ created\033[0m\n"

$(OBJS): obj/%.o: %.c $(INCS) | $(OBJD)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	@printf "\033[32mcompiled: \033[0m$(notdir $<)\n"

$(OBJD):
	@mkdir -p $@
	@printf "\033[32;1m$@ created\033[0m\n"

uninstall: clean
	@rm -rf $(CLIENT)
	@printf "\033[34;1mdeleted $(CLIENT)\033[0m\n"

clean:
	@rm -rf $(OBJD)
	@rm -rf $(CLIENT_WD)/files
	@printf "\033[34;1mdeleted $(OBJD)\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
