#include "client.h"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : str - string that must be checked,
 *           pattern - regex pattern that used to compare,
 *           compile_flags - glib regex setting,
 *           match_flag - glib regex setting,
 * RETURNS : if `str` is valid to `pattern` - TRUE, else
 *           (if regex error too) - FALSE;
 *   NOTES : -
 */
gboolean mx_match(const gchar *str,
                  const gchar *pattern,
                  gint compile_flags,
                  gint match_flag) {
    GRegex *regex;
    GMatchInfo *matchInfo;
    GError *error = NULL;
    gboolean result = FALSE;

    regex = g_regex_new(pattern, compile_flags, match_flag, &error);
    if (error)
        g_warning("Reqex error: %s\n", error->message);
    else {
        g_regex_match_all(regex, str, 0, &matchInfo);
        result = g_match_info_matches(matchInfo) == TRUE;
        g_match_info_free(matchInfo);
        g_regex_unref(regex);
    }
    return result;
}
