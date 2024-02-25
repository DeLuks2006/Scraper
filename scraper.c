#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h> 

size_t write_callback(void *ptr, size_t size, size_t nmemb, char **data) {
  size_t realsize = size * nmemb;

  *data = realloc(*data, realsize+1);
  
  if (*data == NULL) {
    fprintf(stderr, "[-] Failed to realloc memory\n");
    exit(1);
  }
  memcpy(*data, ptr, realsize);
  (*data)[realsize] = '\0'; 
  
  return realsize;
}

void extractLinks(char* html) {
  char *start, *end;
  start = html;

  while((start = strstr(start, "<a href=\""))) {
    start += 9;
    end = strchr(start, '"');
    if (!end) {
      printf("[-] No closing quote found.\n");
      break;
    }

    if (strncmp(start, "http://", 7) == 0 || strncmp(start, "https://", 8) == 0) {
      printf("%.*s\n", (int)(end-start), start);
    }
    start = end + 1;
  }
}

int main() {
  /* ---------[ VARIABLE-DECLARATION ]---------*/
  CURL*     curl;
  CURLcode  res;
  char      website[1024];
  char*     data = NULL;

  /* ---------[ INITIALIZATION-OF-CURL ]---------*/
  curl_global_init(CURL_GLOBAL_ALL);
  printf("[+] Sucessfully initialised CURL.\n");

  curl = curl_easy_init();
  
  if (!curl) {
    printf("[-] Error getting a handle to curl_easy.\n");
    exit(1);
  }
  printf("[+] Got a handle to curl_easy.\n");

  /* ---------[ GETTING-STARTING-POINT ]--------- */
  printf("[i] Enter a URL to start from: ");
  scanf("%s", website);

  curl_easy_setopt(curl, CURLOPT_URL, website);
  printf("[i] Set CURLOPT_URL to \"%s\"\n", website);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

  curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

  /* ---------[ PERFORMING-REQUEST ]--------- */
  res = curl_easy_perform(curl);
  if (res != CURLE_OK){
    fprintf(stderr, "[-] \"curl_easy_perform()\" failed.\n[i] Error: %s\n", curl_easy_strerror(res));
    exit(1);
  }
  printf("[+] Successfully performed GET request!\n");

  /* ---------[ CLEAN-OUTPUT ]--------- */
  extractLinks(data);

  /* ---------[ CLEAN-UP ]---------*/
  printf("[i] Cleaning up...\n");
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  free(data);

  return 0;
}
