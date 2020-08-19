#include "server.h"

#define MX_TEMPLATE_SIZE 10000
#define MX_MESSAGE_SIZE 42

#define MX_ACCOUNT_SID "AC6cf27775aa0531c6c4f1460b571e1475"
#define MX_AUTH_TOKEN "2972522293579be3e9b6b3fea73a3b23"

static void form_message(CURL *curl, char *body) {
  gchar url[MX_TEMPLATE_SIZE];

  g_snprintf(url, sizeof(url), "%s%s%s",
             "https://api.twilio.com/2010-04-01/Accounts/",
             MX_ACCOUNT_SID, "/Messages");
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
  curl_easy_setopt(curl, CURLOPT_USERNAME, MX_ACCOUNT_SID);
  curl_easy_setopt(curl, CURLOPT_PASSWORD, MX_AUTH_TOKEN);
}

static int error_handling(CURL *curl, CURLcode res) {
  long http = 0;

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http);
  curl_easy_cleanup(curl);
  if (res != CURLE_OK) {
      fprintf(stderr, "SMS send failed: %s.\n", curl_easy_strerror(res));
      return -1;
  }
  else if (http != 200 && http != 201) {
      fprintf(stderr, "SMS send failed, HTTP Status Code: %ld.\n", http);
      return -1;
  }
  return 0;
}

gint mx_send_sms(gchar *body) {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  form_message(curl, body);
  res = curl_easy_perform(curl);

  if (error_handling(curl, res)) {
      return -1;
  }
  else {
      fprintf(stderr, "SMS sent successfully!\n");
      return 0;
  }
}
