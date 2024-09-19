/* We rely on wrapped syscalls */
#include "libEpollFuzzer/epoll_fuzzer.h"

#include "App.h"

/* We keep this one for teardown later on */
struct us_listen_socket_t *listen_socket;

/* This test is run by libEpollFuzzer */
void test() {

    {
        /* Keep in mind that fWS::SSLApp({options}) is the same as fWS::App() when compiled without SSL support.
        * You may swap to using fWS:App() if you don't need SSL */
        auto app = fWS::App({
            /* There are example certificates in fastWebSockets.js repo */
            .key_file_name = "../misc/key.pem",
            .cert_file_name = "../misc/cert.pem",
            .passphrase = "1234"
        }).get("/*", [](auto *res, auto *req) {
            auto aborted = std::make_shared<bool>();
            *aborted = false;
            res->onAborted([aborted]() {
                *aborted = true;
            });

            fWS::Loop::get()->defer([res, aborted]() {
                if (!*aborted) {
                    res->cork([res, aborted]() {
                        // Todo: also test upgrade to websocket here
                        res->end("Hello async!");
                    });
                }
            });
        }).listen(9001, [](auto *listenSocket) {
            listen_socket = listenSocket;
        });

        app.run();
    }
    fWS::Loop::get()->free();
}

/* Thus function should shutdown the event-loop and let the test fall through */
void teardown() {
	/* If we are called twice there's a bug (it potentially could if
	 * all open sockets cannot be error-closed in one epoll_wait call).
	 * But we only allow 1k FDs and we have a buffer of 1024 from epoll_wait */
	if (!listen_socket) {
		exit(-1);
	}

	/* We might have open sockets still, and these will be error-closed by epoll_wait */
	// us_socket_context_close - close all open sockets created with this socket context
    if (listen_socket) {
        us_listen_socket_close(0, listen_socket);
        listen_socket = NULL;
    }
}
