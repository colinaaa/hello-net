#include "http/http_server.hh"

#include "http/header.hh"
#include "http/request.hh"
#include "http/response.hh"

#include <docopt.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

static const char USAGE[] =
    R"(HTTP Server.

    Usage:
      http_server [-p <port>] [-d <dir>] [-H <host>]

    Options:
      -h --help                  Show this screen.
      -p <port> --port=<port>    Set port [default: 3000].
      -d <dir> --dir=<dir>       Set dir  [default: .].
      -H <host> --host=<dir>     Set host [default: 127.0.0.1].
)";

int main(int argc, const char **argv) {
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                               {argv + 1, argv + argc},
                                                               true,  // show help if requested
                                                               "HTTP Server by WQY 1.0");
    for (const auto &[k, v] : args) {
        spdlog::info("parsed arg {} = {}", k, v.asString());
    }
    const auto port = args["--port"].asLong();
    const auto dir = args["--dir"].asString();
    const auto host = args["--host"].asString();

    HttpServer app(Address(host, port));

    const auto pwd = std::filesystem::relative(dir);

    const auto handler = [&pwd](const Request &req) {
        const std::string path = pwd.string() + req.path();
        spdlog::info("handle request for {}", path);

        if (!std::filesystem::exists(path)) {
            spdlog::error("file {} not exists", path);
            return Response(Response::Status::NOT_FOUND).body("Not Found");
        }

        try {
            std::ifstream t(std::move(path));
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            spdlog::info("read file {}: {}", path, str);

            return Response(Response::Status::OK).header("key", "value").header("path", std::move(path)).body(str);
        } catch (const std::exception &e) {
            spdlog::error("read file error {}", e.what());
            return Response(Response::Status::NOT_FOUND).body("Not Found");
        }
    };

    spdlog::info("start serve at {}:{} on {}", host, port, pwd.string());
    app.serve(handler);
    return 0;
}
