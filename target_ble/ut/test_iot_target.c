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


#include "unity.h"
#include <string.h>
#include <stdlib.h>

#include "target_ble.h"

static int num_errs = 0;
void event_cb(void *context, ble_event_t *event) {
    switch(event->type) {
        case BLE_ERROR:
            printf("Error!\n");
            num_errs += 1;
        case BLE_ADVERTISED:
            printf("Recieved advertise!\n");
            const ble_advertised_t *adv = &event->op.advertise;
            printf("data from: [%s]\n", adv->contents->mac);
        default:
            printf("Case : [%d] not implemented.\n", event->type);
    }
}

void test_init_and_teardown(void)
{
    struct ev_loop *Loop;
	bool ret = false;	
	void *ctx = NULL;
	void *caller_ctx = NULL;
    Loop = EV_DEFAULT;

    ret = ble_init(
            &ctx,
            caller_ctx,
            Loop, 
            event_cb);

    TEST_ASSERT_TRUE_MESSAGE(ret, "Should have initialized the target layer.");

    ble_exit(ctx);
}

int main(int argc, char *argv[])
{

    UNITY_BEGIN();
    RUN_TEST(test_init_and_teardown);
    return UNITY_END();
}
