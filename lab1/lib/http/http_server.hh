#ifndef LIB_HTTP_SERVER_HH
#define LIB_HTTP_SERVER_HH

#include "http/request.hh"
#include "http/response.hh"
#include "server.hh"

#include <functional>

class HttpServer : public Server {
  private:
    using HandlerT = std::function<Response(const Request &)>;
    /* data */
  public:
    HttpServer() = delete;
    HttpServer(const uint16_t port) : Server(port) {}
    HttpServer(const Address &addr) : Server(addr) {}

    void serve(HandlerT handler) {
        bind();
        listen();

        for (;;) {
            auto conn = accept();
            spdlog::info("got connection from {}:{}", conn.peer_address().ip(), conn.peer_address().port());
            std::string buf{};
            conn.read(buf);

            Response resp = handler(buf);
            conn.write(resp.to_string(), true);
        }
    }
};

#endif
