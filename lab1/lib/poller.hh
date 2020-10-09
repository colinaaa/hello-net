#ifndef LIB_POLLER_HH
#define LIB_POLLER_HH

#include "epoll_event.hh"
#include "file_descriptor.hh"
#include "util.hh"

#include <array>
#include <cstdlib>
#include <functional>
#include <list>
#include <sys/epoll.h>
#include <unistd.h>

//! Waits for events on file descriptors and executes corresponding callbacks.
class Poller {
  public:
    //! Indicates interest in reading (In) or writing (Out) a polled fd.
    enum class Direction : short {
        In = EPOLLIN,   //!< Callback will be triggered when Rule::fd is readable.
        Out = EPOLLOUT  //!< Callback will be triggered when Rule::fd is writable.
    };

    Poller() : _epollfd(SystemCall("EpollCreate1", epoll_create1(0))) {}

    ~Poller() {
        if (not _epollfd.closed()) {
            _epollfd.close();
        }
    }

  private:
    FileDescriptor _epollfd;

  public:
    //! Add a rule whose callback will be called when `fd` is ready in the specified Direction.
    void add(const FileDescriptor &fd, const Direction direction, bool edge_trigger = true);

    void del(const FileDescriptor &fd) {
        SystemCall("EpollCtl", epoll_ctl(_epollfd.fd_num(), EPOLL_CTL_DEL, fd.fd_num(), nullptr));
    }

    void mod(const FileDescriptor &fd, const uint32_t event) {
        epoll_event ev;
        ev.events = event;
        SystemCall("EpollCtl", epoll_ctl(_epollfd.fd_num(), EPOLL_CTL_MOD, fd.fd_num(), &ev));
    }

    //! Calls [epoll(2)](\ref man2::epoll) and then executes callback for each ready fd.
    std::vector<EpollEvent> wait(const int timeout_ms);
};

using Direction = Poller::Direction;

//! \class Poller
//!
//! An Poller holds a std::list of Rule objects. Each time Poller::wait_next_event is
//! executed, the Poller uses the Rule objects to construct a call to [poll(2)](\ref man2::poll).
//!
//! When a Rule is installed using Poller::add_rule, it will be polled for the specified Rule::direction
//! whenver the Rule::interest callback returns `true`, until Rule::fd is no longer readable
//! (for Rule::direction == Direction::In) or writable (for Rule::direction == Direction::Out).
//! Once this occurs, the Rule is canceled, i.e., the Poller deletes it.
//!
//! A Rule installed using Poller::add_cancelable_rule will be polled and canceled under the
//! same conditions, with the additional condition that if Rule::callback returns `true`, the
//! Rule will be canceled.

#endif  // LIB_POLLER_HH