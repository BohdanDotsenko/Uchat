CLIENT = uchat
SERVER = uchat_server

CLIENTD = client
SERVERD = server
DB = uchat_db

CLIENTO = $(CLIENTD)/$(CLIENT)
SERVERO = $(SERVERD)/$(SERVER)

all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENTO)
$(SERVER): $(SERVERO)

$(CLIENTO): $(CLIENTD)
$(SERVERO): $(SERVERD)

$(CLIENTO) $(SERVERO):
	@make -sC $<
	@cp $@ .


uninstall: clean
	@make -sC $(CLIENTD) $@
	@make -sC $(SERVERD) $@


clean:
	@rm -rf $(CLIENT)
	@rm -rf $(SERVER)
	@rm -rf $(DB)
	@make -sC $(CLIENTD) $@
	@make -sC $(SERVERD) $@

reinstall: uninstall all

.PHONY: + uninstall clean reinstall $(CLIENTO) $(SERVERO)
