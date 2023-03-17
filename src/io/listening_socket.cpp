#include "listening_socket.hpp"

#include <system_error>

#include <sys/fcntl.h>
namespace sys {
#include <sys/socket.h>
#include <netinet/in.h>
} // namespace sys
#include <unistd.h>
#include <cstring>
namespace io {

listening_socket::listening_socket()
    : fd_(sys::socket(AF_INET, sys::SOCK_STREAM, 0)) {
    fcntl(fd_, F_SETFL, O_NONBLOCK);
    int on = 1;
    int rc = sys::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    if (rc != 0)
        throw std::system_error(std::error_code(errno, std::system_category()));
}

listening_socket::~listening_socket() {
    if (fd_ > 0)
        close(fd_);
}
listening_socket::listening_socket(listening_socket&& other)
    : fd_(other.fd_) {
    other.fd_ = 0;
}
auto listening_socket::operator=(listening_socket&& other) -> listening_socket& {
    fd_ = other.fd_;
    other.fd_ = 0;
    return *this;
}

auto listening_socket::bind(int port) -> void {
    struct sys::sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = sys::htons(0 /*INADDR_ANY*/);
    addr.sin_port = sys::htons(port);
    int rc = sys::bind(fd_, (struct sys::sockaddr*)&addr, sizeof(addr));
    if (rc != 0)
        throw std::system_error(std::error_code(errno, std::system_category()));
}

auto listening_socket::listen() -> void {
    int rc = sys::listen(fd_, SOMAXCONN);
    if (rc != 0)
        throw std::system_error(std::error_code(errno, std::system_category()));
}

} // namespace io
