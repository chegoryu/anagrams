#include "websockets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* main_page;
int main_len;

void ws_open(ws_s* ws) {
  fprintf(stderr, "Opened a new websocket connection (%p)\n", (void*)ws);
  for (int i = 0; i != 20; ++i) {
    websocket_write(ws, websocket_udata(ws), strlen((char*)websocket_udata(ws)), 1);
    usleep(500000);
  }
  websocket_close(ws);
}

void ws_close(ws_s* ws) {
  //  free(websocket_udata(ws));
  fprintf(stderr, "Closed websocket connection (%p)\n", (void*)ws);
}

void on_request(http_request_s* request) {
  fprintf(stderr, "request\n");
  
  http_response_s* response = http_response_create(request);
  http_response_log_start(response); // logging
  
  // websocket upgrade.
  if (request->upgrade) {
    //    if (strcmp(request->path, "/ws") == 0) {
      int len = strlen(request->query);
      char* query = malloc(len + 1);
      memcpy(query, request->query, len + 1);
      websocket_upgrade(.request = request,
                        .on_open = ws_open, .on_close = ws_close,
                        .timeout = 40, .response = response,
                        .udata = query);
      //    }
    return;
  }
  
  // HTTP response
  //  if (strcmp(request->path, "/") == 0)
    http_response_write_body(response, main_page, main_len);
    //  else {
    //    response->status = 404;
    //    const char* e404 = "404, there is no such page";
    //    http_response_write_body(response, e404, strlen(e404));
    //  }
  
  http_response_finish(response);
}

int init() {
  FILE* f = fopen("index.html", "r");
  fseek(f, 0L, SEEK_END);

  main_len = ftell(f);
  main_page = malloc(main_len + 1);

  fseek(f, 0L, SEEK_SET);
  int read = fread(main_page, 1, main_len, f);
  fclose(f);

  return read != main_len ? -1 : 0;
}

#define THREAD_COUNT 4
int main(void) {
  int errno;
  if ((errno = init()) != 0)
    return errno;
  
  http_listen("3000", NULL, .on_request = on_request, .log_static = 1);
  facil_run(.threads = THREAD_COUNT);
  return 0;
}
