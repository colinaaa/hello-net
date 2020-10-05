#ifndef LIB_UTIL_HH
#define LIB_UTIL_HH

#include <string>
#include <system_error>

//! std::system_error plus the name of what was being attempted
class tagged_error : public std::system_error {
  private:
    std::string _attempt_and_error;  //!< What was attempted, and what happened

  public:
    //! \brief Construct from a category, an attempt, and an error code
    //! \param[in] category is the category of error
    //! \param[in] attempt is what was supposed to happen
    //! \param[in] error_code is the resulting error
    tagged_error(const std::error_category &category, const std::string &attempt, const int error_code)
        : system_error(error_code, category), _attempt_and_error(attempt + ": " + std::system_error::what()) {}

    //! Returns a C string describing the error
    const char *what() const noexcept override { return _attempt_and_error.c_str(); }
};

//! a tagged_error for syscalls
class unix_error : public tagged_error {
  public:
    //! brief Construct from a syscall name and the resulting errno
    //! \param[in] attempt is the name of the syscall attempted
    //! \param[in] error is the [errno(3)](\ref man3::errno) that resulted
    explicit unix_error(const std::string &attempt, const int error = errno)
        : tagged_error(std::system_category(), attempt, error) {}
};

//! Error-checking wrapper for most syscalls
int SystemCall(const char *attempt, const int return_value, const int errno_mask = 0);

//! Version of SystemCall that takes a C++ std::string
int SystemCall(const std::string &attempt, const int return_value, const int errno_mask = 0);

//! Get the time in milliseconds since the program began.
uint64_t timestamp_ms();

#endif  // LIB_UTIL_HH
