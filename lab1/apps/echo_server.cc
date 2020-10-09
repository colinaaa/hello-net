#include "server.hh"
#include "socket.hh"

#include <iostream>
#include <sys/signal.h>

int main() {
    constexpr uint port = 3000;
    Server app(port);

    const auto handler = [](TCPSocket &&sock) {
        do {
            const auto &&content = sock.read();

            sock.write(content, true);
        } while (not sock.eof());

        spdlog::info("connection closed, fd = {}", sock.fd_num());

        sock.close();
    };

    spdlog::info("start serve at {}", port);
    app.serve(handler);
    return 0;
}
