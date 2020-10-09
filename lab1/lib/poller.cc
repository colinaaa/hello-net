#include "poller.hh"

#include "util.hh"

#include <array>

void Poller::add(const FileDescriptor &fd, const Direction direction, bool edge_trigger) {
    epoll_event ev;
    ev.events = static_cast<uint32_t>(direction);

    if (edge_trigger) {
        ev.events |= EPOLLET;
    }

    SystemCall("EpollCtl", ::epoll_ctl(_epollfd.fd_num(), EPOLL_CTL_ADD, fd.fd_num(), &ev));
}

std::vector<EpollEvent> Poller::wait(const int timeout_ms) {
    constexpr uint max_events = 1000;
    std::array<epoll_event, max_events> events{};

    const int n = SystemCall("EpollWait", ::epoll_wait(_epollfd.fd_num(), events.data(), max_events, timeout_ms));

    return {events.begin(), events.begin() + n};
}