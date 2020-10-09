#include "poller.hh"
#include "server.hh"

#include <functional>

class PollServer : public Server {
  public:
    enum class Type : short { Read, Write };

  private:
    using HandlerT = std::function<void(std::string &)>;
    void register_handler(const HandlerT &handler, const Type t);

    std::list<HandlerT> read_handlers{};
    std::list<HandlerT> write_handlers{};

    Poller _poller;

  public:
    PollServer(const Address &addr) : Server(addr), _poller() {}
    PollServer(const uint16_t port) : Server(port), _poller() {}

    void onRead(const HandlerT &handler) { register_handler(handler, Type::Read); }
    void onWrite(const HandlerT &handler) { register_handler(handler, Type::Write); }
    void serve();
};

void PollServer::serve() {
    bind();
    listen();

    for (;;) {
        const auto events = _poller.wait(-1);

        for (const EpollEvent &ev : events) {
            if (ev.fd_num() == listen_fdnum()) {
                const auto &&conn = accept();

                _poller.add(conn, Poller::Direction::In);
            } else if (ev.is_read_event()) {
            } else if (ev.is_write_event()) {
            }
        }
    }
}