/*
Copyright (c) 2020, Charter Communications Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. Neither the name of the Charter Communications Inc. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Charter Communications Inc. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "target_ble.h"

static struct ev_loop *Loop;
static void           *context;


#define MAC_COUNT 1
ble_mac_t whitelist_macs[1] = {
    "18:93:D7:76:1D:AD"
};

#define SERVICE_COUNT 2
ble_uuid_t whitelist_services[2] = {
    "00001810-0000-1000-8000-00805F9B34FB",
    "0000FFF0-0000-1000-8000-00805F9B34FB"
};

#define CHAR_COUNT 1
ble_uuid_t whitelist_characteristics[1] = {
    "00002A35-0000-1000-8000-00805F9B34FB"
};

// This will setup the scan parameters for the white listed devices
ble_discovery_scan_params_t params = {
    .scan_interval_ms = 20,
    .scan_duration_ms = 20,
    .is_passive = false,
    .uuid_filter = whitelist_services,
    .num_uuid_filters = SERVICE_COUNT
};

void event_cb(void *context, const ble_event_t *event) {
    switch(event->type) {
        case BLE_ERROR:
            printf("Error!\n");
        case BLE_AVERTISED:
            printf("Recieved advertise!\n");
            const ble_advertised_t *adv = &event->op.advertise;
            printf("data from: [%s]\n", adv->contents->mac);
        default:
            printf("Case : [%d] not implemented.\n", event->type);
    }
}


void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    // Passing
}

int main(int argc, char *argv[])
{

    Loop = EV_DEFAULT;
    struct ev_io w_accept;
    int sd;
    bool ret = false;
    context = NULL;


    ev_io_init(&w_accept, accept_cb, sd, EV_READ);
    ev_io_start(Loop, &w_accept);

    ret = ble_init(&context, Loop, &event_cb);

    if (!ret) {
        printf("Should have initialzied the target layer. Cannot continue.\n");
        return -1;
    }

    return 0;
}
