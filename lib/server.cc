#include "server.hh"

#include "poller.hh"
#include "socket.hh"

#include <iostream>

void Server::serve(const HandlerT &handler) {
    _sock.bind(_listen_addr);

    _sock.listen();

    for (;;) {
        auto &&conn = _sock.accept();

        spdlog::info("got connection from {}:{}", conn.peer_address().ip(), conn.peer_address().port());

        handler(std::move(conn));
    }
}

void Server::serve(const Address &addr, const HandlerT &handler) {
    _listen_addr = std::move(addr);
    serve(handler);
}