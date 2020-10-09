#ifndef LIB_HTTP_REQUEST_HH
#define LIB_HTTP_REQUEST_HH

#include "http/header.hh"
#include "spdlog/spdlog.h"

#include <scn/scn.h>
#include <stdexcept>
#include <string>

class Request {
  private:
    enum class Method : short { GET, POST };
    HeaderList _headers{};
    Method _method;
    std::string _body{};
    std::string _path{};
    const std::string version{"HTTP/0.9"};

  public:
    Request() = default;

    Request(const Method &method) : _method(method) {}
    Request(const Method &method, const std::string &body) : _method(method), _body(body) {}
    Request(const std::string &raw) : Request(parse(raw)) {}

    auto path() const { return _path; }

    static std::string method_to_string(const Method &method) {
        switch (method) {
            case Method::GET:
                return "GET";
            case Method::POST:
                return "POST";
            default:
                return "";
        }
    }

    static Method string_to_method(const std::string &str) {
        if (str == "GET") {
            return Method::GET;
        }
        if (str == "POST") {
            return Method::POST;
        }

        return Method::GET;
    }

    Request &header(const std::string &&key, const std::string val) {
        _headers.push_back(key, val);
        return *this;
    }

    Request &body(const std::string &body) {
        if (not _body.empty()) {
            throw std::runtime_error("http: request: body already setted");
        }
        _body = body;
        return *this;
    }

    static Request parse(const std::string_view &raw) {
        Request req{};
        const std::string delim = "\r\n";
        size_t pos = 0;
        std::string_view token;

        const auto end = raw.find("\r\n\r\n");
        std::string version;
        std::string method;
        std::string path;
        scn::scan(raw, "{} {} HTTP/{}", method, path, version);
        req._path = path;

        spdlog::info("parse result: {} {} {}", method, path, version);

        while ((pos = raw.find(delim, pos)) != end) {
            token = raw.substr(pos);

            const size_t kEnd = token.find_first_of(':');
            const size_t vEnd = token.find_first_of("\r\n", kEnd);

            //  omit \r\n & :
            std::string key{token.substr(2, kEnd - 2)};
            std::string val{token.substr(kEnd + 2, vEnd - kEnd - 1)};
            spdlog::info("parse header key = {}, val = {}", key, val);

            req._headers.push_back(key, val);

            pos = pos + delim.length();
        }

        req._body = std::string(raw.substr(pos));

        return req;
    }

    std::string to_string() const {
        return fmt::format(
            "{} {} {}\r\n{}\r\n\r\n{}", method_to_string(_method), _path, version, _headers.to_string(), _body);
    }
};

#endif  // LIB_HTTP_REQUEST_HH
