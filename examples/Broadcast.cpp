/* We simply call the root header file "App.h", giving you fWS::App and fWS::SSLApp */
#include "App.h"
#include <time.h>
#include <iostream>

/* This is a simple WebSocket echo server example.
 * You may compile it with "WITH_OPENSSL=1 make" or with "make" */

fWS::SSLApp *globalApp;

int main() {
    /* ws->getUserData returns one of these */
    struct PerSocketData {
        /* Fill with user data */
    };

    /* Keep in mind that fWS::SSLApp({options}) is the same as fWS::App() when compiled without SSL support.
     * You may swap to using fWS:App() if you don't need SSL */
    fWS::SSLApp app = fWS::SSLApp({
        /* There are example certificates in fastWebSockets.js repo */
	    .key_file_name = "misc/key.pem",
	    .cert_file_name = "misc/cert.pem",
	    .passphrase = "1234"
	}).ws<PerSocketData>("/*", {
        /* Settings */
        .compression = fWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024 * 1024,
        .idleTimeout = 16,
        .maxBackpressure = 1 * 1024 * 1024,
        .closeOnBackpressureLimit = false,
        .resetIdleTimeoutOnSend = false,
        .sendPingsAutomatically = true,
        /* Handlers */
        .upgrade = nullptr,
        .open = [](auto *ws) {
            /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
            ws->subscribe("broadcast");
        },
        .message = [](auto */*ws*/, std::string_view /*message*/, fWS::OpCode /*opCode*/) {

        },
        .drain = [](auto */*ws*/) {
            /* Check ws->getBufferedAmount() here */
        },
        .ping = [](auto */*ws*/, std::string_view) {
            /* Not implemented yet */
        },
        .pong = [](auto */*ws*/, std::string_view) {
            /* Not implemented yet */
        },
        .close = [](auto */*ws*/, int /*code*/, std::string_view /*message*/) {
            /* You may access ws->getUserData() here */
        }
    }).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    });

    struct us_loop_t *loop = (struct us_loop_t *) fWS::Loop::get();
    struct us_timer_t *delayTimer = us_create_timer(loop, 0, 0);

    // broadcast the unix time as millis every 8 millis
    us_timer_set(delayTimer, [](struct us_timer_t */*t*/) {

        struct timespec ts;
        timespec_get(&ts, TIME_UTC);

        int64_t millis = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

        //std::cout << "Broadcasting timestamp: " << millis << std::endl;

        globalApp->publish("broadcast", std::string_view((char *) &millis, sizeof(millis)), fWS::OpCode::BINARY, false);

    }, 8, 8);

    globalApp = &app;

    app.run();
}
