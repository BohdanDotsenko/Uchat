#include "client.h"

void mx_apply_styles(const gchar *path_to_css) {
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path (provider, path_to_css, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
}
