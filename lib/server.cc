#include "server.hh"

#include "poller.hh"
#include "socket.hh"

void Server::serve(const HandlerT &handler) {
    _sock.bind(_listen_addr);

    _sock.listen();

    for (;;) {
        auto &&conn = _sock.accept();

        handler(std::move(conn));
    }
}

void Server::serve(const Address &addr, const HandlerT &handler) {
    _listen_addr = std::move(addr);
    serve(handler);
}