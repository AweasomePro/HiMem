//
// Created by kewen on 2019-08-28.
//

#include <string.h>
#include "looper_test.h"
#include "looper.h"
#include "../log.h"

#define LOG_TAG "looper_test"

static message_looper_t *looper;

static void _handle(message_t *msg) {
    LOGI(" -----> %s", (char *) msg->data);
}

void test_looper_start() {
    looper = looperCreate(_handle);
    if (looper == NULL) {
        LOGI("looperCreate fail!!!!!!!!!");
        return;
    }

    if (looperStart(looper) == LOOPER_START_THREAD_ERROR) {
        LOGI("looperStart thread create fail!!!!!!!!!");
        looperDestroy(&looper);
        return;
    }
}

void test_looper_destroy() {
    looperDestroy(&looper);
}

void test_looper_push(char *data) {
    looperPost(looper, 0, data, strlen(data) + 1);
}