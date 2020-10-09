#ifndef LIB_HTTP_RESPONSE_HH
#define LIB_HTTP_RESPONSE_HH

#include "http/header.hh"

#include <scn/scn.h>

class Response {
  public:
    enum class Status : short { OK = 200, NOT_FOUND = 404 };

  private:
    Status _status;
    HeaderList _headers{};
    std::string _body{};
    const std::string version{"HTTP/0.9"};

  public:
    Response(const Status status) : _status(status) {}

    Response &header(const std::string &&key, const std::string &&val) {
        _headers.push_back(key, val);
        return *this;
    }

    Response &body(const std::string &body) {
        _body = body;
        return *this;
    }

    static std::string status_message(const Status status) {
        switch (status) {
            case Status::OK:
                return "OK";
            case Status::NOT_FOUND:
                return "NOT FOUND";
            default:
                return "";
        }
    }

    std::string to_string() const {
        return fmt::format("{} {} {}\r\n{}\r\n\r\n{}",
                           version,
                           std::to_string(static_cast<int>(_status)),
                           status_message(_status),
                           _headers.to_string(),
                           _body);
    }
};

#endif  // LIB_HTTP_RESPONSE_HH