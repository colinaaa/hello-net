#ifndef LIB_HTTP_HEADER_HH
#define LIB_HTTP_HEADER_HH

#include <initializer_list>
#include <list>
#include <spdlog/fmt/fmt.h>
#include <string>
#include <vector>

class Header {
  private:
    std::string _key;
    std::string _value;

  public:
    Header() = default;
    Header(const std::string &key, const std::string &value) : _key(key), _value(value) {}

    std::string to_string() const { return fmt::format("{}: {}", _key, _value); }
};

class HeaderList {
  private:
    std::list<Header> _list;

  public:
    HeaderList() = default;
    HeaderList(const std::initializer_list<Header> &&ilist) : _list(ilist) {}
    HeaderList(const std::vector<Header> &&list) : _list(list.cbegin(), list.cend()) {}

    void push_back(const std::string &key, const std::string &value) { _list.emplace_back(key, value); }

    void join(std::string &out) const {
        for (const auto &h : _list) {
            out += h.to_string() + "\r\n";
        }
    }

    std::string join() const {
        std::string ret{};
        join(ret);
        return ret;
    }

    std::string to_string() const { return join(); }
};

#endif  // LIB_HTTP_HEADER_HH