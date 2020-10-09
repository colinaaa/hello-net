#ifndef LIB_EPOLL_EVENT_HH
#define LIB_EPOLL_EVENT_HH

#include <memory>
#include <sys/epoll.h>

class EpollEvent {
  private:
    std::shared_ptr<epoll_event> _event;

  public:
    EpollEvent(const epoll_event &ev) : _event(std::make_shared<epoll_event>(ev)) {}

    auto events() const { return _event->events; }

    auto fd_num() const { return _event->data.fd; }

    bool is_read_event() const { return events() & EPOLLIN; }
    bool is_write_event() const { return events() & EPOLLOUT; }
};  // class EpollEvent

#endif  // LIB_EPOLL_EVENT_HH