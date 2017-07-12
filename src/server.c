#include "websockets.h"
#include <unicode/utf8.h>
#include <unicode/utypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "runner.h"

char* main_page;
int main_len;

const char* e404 = "404, there is no such page";
const char* ebad = "Bad request";
const char* eunexpected = "[unexpected error]";

// VALID ONLY FOR DEMO
struct request_info {
    ws_s* ws;
    int num_found;
    char tmpbuf[CONFIG_REQUEST_MAX];
};

int match_callback(const char* data, int words, int* length, void* info_) {
    struct request_info* info = (struct request_info*)info_;
    
    if (info->num_found++ >= CONFIG_MAX_RESPONCE)
        return 0;
    
    int inptr = 0;
    int outptr = 0;
    UBool err = 0;
    for (int w = 0; w != words; ++w) {
        if (outptr == sizeof(info->tmpbuf))
            goto fail;

        if (w != 0)
            U8_APPEND((uint8_t*)info->tmpbuf, outptr, sizeof(info->tmpbuf), ' ', err);

        if (err)
            goto fail;

        for (int i = 0; i != length[w]; ++i, ++inptr) {
            U8_APPEND((uint8_t*)info->tmpbuf, outptr, sizeof(info->tmpbuf), data[inptr] + CONFIG_UNI_START, err);
            if (err)
                goto fail;
        }
    }
    websocket_write(info->ws, info->tmpbuf, outptr, 1);
    return 1;
fail:
    websocket_write(info->ws, (char*)eunexpected, strlen(eunexpected), 1);
    return 0;
}

void ws_open(ws_s* ws) {
    fprintf(stderr, "Opened a new websocket connection (%p)\n", (void*)ws);
    
    if (websocket_udata(ws) != NULL) {
        const char* udata       = websocket_udata(ws);
        int32_t udata_len = strlen(udata);
        
        char* request = malloc(udata_len + 1);
        int32_t inptr = 0, outptr = 0;
        
        while (inptr != udata_len) {
            UChar32 ch;
            U8_NEXT(udata, inptr, udata_len, ch);
            if (ch < 0) { 
                free(request);
                goto fail;
            }

            ch = u_tolower(ch);
            
            if (CONFIG_UNI_START <= u_tolower(ch) && u_tolower(ch) < CONFIG_UNI_START + 33)
                request[outptr++] = u_tolower(ch) - CONFIG_UNI_START;
            else if (ch != ' ') {
                free(request);
                goto fail;
            }
        }

        struct request_info r_info;
        r_info.ws        = ws;
        r_info.num_found = 0;
        brute(request, outptr, 1, 10, match_callback, &r_info, 0, 0, CONFIG_MAX_TIME);

        free(request);
        websocket_close(ws);
        return;
    }
fail:;
    websocket_write(ws, (char*)ebad, strlen(ebad), 1);
    websocket_close(ws);
}

void ws_close(ws_s* ws) {
    free(websocket_udata(ws));
    fprintf(stderr, "Closed websocket connection (%p)\n", (void*)ws);
}

void on_request(http_request_s* request) {
    fprintf(stderr, "request\n");
    
    http_response_s* response = http_response_create(request);
    http_response_log_start(response); // logging
    
    // websocket upgrade.
    if (request->upgrade) {
        if (strcmp(request->path, "/ws") == 0) {
            char* udata = NULL;
            if (strncmp(request->query, "q=", 2) == 0 && strlen(request->query) < CONFIG_REQUEST_MAX) {
                udata = malloc(strlen(request->query) + 1);
                http_decode_path_unsafe(udata, request->query + 2);
            }
            
            websocket_upgrade(.request = request,
                              .on_open = ws_open, .on_close = ws_close,
                              .timeout = 40, .response = response,
                              .udata = udata);
        }
        return;
    }
    
    // HTTP response
    if (strcmp(request->path, "/") == 0)
        http_response_write_body(response, main_page, main_len);
    else {
        response->status = 404;
        http_response_write_body(response, e404, strlen(e404));
    }
    
    http_response_finish(response);
}

int init() {
    initBrute(CONFIG_DICT_PATH);
    
    FILE* f = fopen("index.html", "r");
    fseek(f, 0L, SEEK_END);

    main_len = ftell(f);
    main_page = malloc(main_len + 1);

    fseek(f, 0L, SEEK_SET);
    int read = fread(main_page, 1, main_len, f);
    fclose(f);

    return read != main_len ? -1 : 0;
}

int main(void) {
    int errno;
    if ((errno = init()) != 0)
        return errno;
  
    http_listen(CONFIG_SERVER_LOCATION, NULL, .on_request = on_request, .log_static = 1);
    facil_run(.threads = CONFIG_THREAD_COUNT);

    delBrute();
    return 0;
}
