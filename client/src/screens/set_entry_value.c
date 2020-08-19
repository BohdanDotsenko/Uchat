#include "client.h"

void mx_set_entry_value(GtkBuilder *builder, const gchar *entry_name,
                        const gchar *value) {
    GtkEntry *entry = NULL;
    GtkEntryBuffer* buf = NULL;

    entry = GTK_ENTRY(gtk_builder_get_object(builder, entry_name));
    buf = gtk_entry_get_buffer(entry);
    gtk_entry_buffer_set_text(buf, value, -1);
}
