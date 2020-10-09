#ifndef APP_SERVER_HH
#define APP_SERVER_HH

#include "address.hh"
#include "poller.hh"
#include "socket.hh"

#include <functional>
#include <spdlog/spdlog.h>

class Server {
    using HandlerT = std::function<void(TCPSocket &&)>;

  private:
    Address _listen_addr;
    TCPSocket _sock{};

  public:
    Server() = delete;
    Server(const Address &addr) : _listen_addr(addr) {}
    Server(const uint16_t port) : _listen_addr(Address("0.0.0.0", port)) {}

    //! \brief Run forever
    void serve(const HandlerT &handler);

    //! \brief Run forever on addr
    void serve(const Address &addr, const HandlerT &handler);

    auto bind() { return _sock.bind(_listen_addr); }

    auto listen() { return _sock.listen(); }

    auto listen_fdnum() const { return _sock.fd_num(); }

    auto accept() { return _sock.accept(); }

    auto write(BufferViewList buffer, bool write_all = true) { return _sock.write(buffer, write_all); }
};  // class Server

#endif  // APP_SERVER_HH