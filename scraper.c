#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void extractLinks(char* html);

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

void anotherOne(char* link) {
  CURL *curl;
  CURLcode res;
  char* data = NULL;

  curl = curl_easy_init();
  
  curl_easy_setopt(curl, CURLOPT_URL, link);
  printf("[i] Set CURLOPT_URL to \"%s\"\n", link);
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
  sleep(5);
  extractLinks(data);

  /* ---------[ CLEAN-UP ]--------- */
  printf("[i] Cleaning up...\n");
  curl_easy_cleanup(curl);
  free(data);
}

void extractLinks(char* html) {
  char *start, *end;

  /* ---------[ GRABBING-FULL-LINKS ]--------- */
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
      
      char linky[end - start + 1];
      strncpy(linky, start, end - start);
      linky[end - start] = '\0';
      anotherOne(linky);
    } else {
      ;
    }
    start = end + 1;
  }

  /* ---------[ GRABBING-RELATIVE-LINKS ]--------- */
  start = html;
  
  printf("\n-- RELATIVE LINKS --\n");
  while((start = strstr(start, "a href=\""))) {
    start +=9;
    end = strchr(start, '"');
    if (!end) {
      printf("[-] No closing quote found");
    }
    
    printf("%.*s\n", (int)(end-start), start);
    start = end+1;
  }
  
  printf("\n------------------------------------------------------------\n");
}

int main() {
  /* ---------[ VARIABLE-DECLARATION ]--------- */
  char      website[1024];
 /* ---------[ GETTING-STARTING-POINT ]--------- */
  printf("[i] Enter a URL to start from: ");
  scanf("%s", website);
  anotherOne(website);

  return 0;
}
