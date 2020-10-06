#ifndef APP_SERVER_HH
#define APP_SERVER_HH

#include "address.hh"
#include "poller.hh"
#include "socket.hh"

#include <functional>

class Server {
    using HandlerT = std::function<void(TCPSocket)>;

  private:
    Address _listen_addr;
    TCPSocket _sock;

  public:
    Server() = delete;
    Server(const Address &addr) : _listen_addr(addr), _sock() {}
    Server(const uint16_t port) : _listen_addr(Address("0.0.0.0", port)), _sock() {}

    //! \brief Run forever
    void serve(const HandlerT &handler);

    //! \brief Run forever on addr
    void serve(const Address &addr, const HandlerT &handler);
};  // class Server

#endif  // APP_SERVER_HH