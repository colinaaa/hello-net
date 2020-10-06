#include "server.hh"
#include "socket.hh"

#include <iostream>

int main() {
    Server app(3000);

    const auto handler = [](TCPSocket sock) {
        do {
            const auto &&content = sock.read();

            std::cout << "[info]: get content: " << content << std::endl;

            sock.write(content, true);
        } while (not sock.eof());

        std::cout << "[info]: connection closed" << std::endl;

        sock.close();
    };

    app.serve(handler);
    return 0;
}
