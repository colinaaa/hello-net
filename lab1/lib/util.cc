#include "util.hh"

#include <array>
#include <cctype>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/socket.h>

//! \returns the number of milliseconds since the program started
uint64_t timestamp_ms() {
    using time_point = std::chrono::steady_clock::time_point;
    static const time_point program_start = std::chrono::steady_clock::now();
    const time_point now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - program_start).count();
}

//! \param[in] attempt is the name of the syscall to try (for error reporting)
//! \param[in] return_value is the return value of the syscall
//! \param[in] errno_mask is any errno value that is acceptable, e.g., `EAGAIN` when reading a non-blocking fd
//! \details This function works for any syscall that returns less than 0 on error and sets errno:
//!
//! For example, to wrap a call to [open(2)](\ref man2::open), you might say:
//!
//! ~~~{.cc}
//! const int foo_fd = SystemCall("open", ::open("/tmp/foo", O_RDWR));
//! ~~~
//!
//! If you don't have permission to open the file, SystemCall will throw a std::runtime_error.
//! If you don't want to throw in that case, you can pass `EACCESS` in `errno_mask`:
//!
//! ~~~{.cc}
//! // open a file, or print an error if permission was denied
//! const int foo_fd = SystemCall("open", ::open("/tmp/foo", O_RDWR), EACCESS);
//! if (foo_fd < 0) {
//!     std::cerr << "Access to /tmp/foo was denied." << std::endl;
//! }
//! ~~~
int SystemCall(const char *attempt, const int return_value, const int errno_mask) {
    if (return_value >= 0 || errno == errno_mask) {
        return return_value;
    }

    throw unix_error(attempt);
}

//! \param[in] attempt is the name of the syscall to try (for error reporting)
//! \param[in] return_value is the return value of the syscall
//! \param[in] errno_mask is any errno value that is acceptable, e.g., `EAGAIN` when reading a non-blocking fd
//! \details see the other SystemCall() documentation for more details
int SystemCall(const std::string &attempt, const int return_value, const int errno_mask) {
    return SystemCall(attempt.c_str(), return_value, errno_mask);
}
