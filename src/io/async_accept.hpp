#pragma once

#include "io/io_context.hpp"
#include "io/listening_socket.hpp"
#include "io/connection.hpp"
#include <profiling.hpp>

#include <sys/fcntl.h>
namespace sys {
#include <sys/socket.h>
} // namespace sys

namespace io {

namespace detail {

struct async_accept_sender {
    io_context* ctx_;
    native_file_desc_t fd_;

    using completion_signatures = stdexec::completion_signatures< //
            stdexec::set_value_t(connection),                     //
            stdexec::set_error_t(std::system_error),              //
            stdexec::set_stopped_t()>;

    template <stdexec::receiver Recv>
    class oper : oper_body_base {
        Recv recv_;
        io_context* ctx_;
        native_file_desc_t fd_;

        auto try_run() noexcept -> bool override {
            PROFILING_SCOPE_N("async_accept::try_run");
            sys::sockaddr address;
            socklen_t length = sizeof(address);
            int rc = sys::accept(fd_, &address, &length);
            PROFILING_SET_TEXT_FMT(32, "fd=%d => %d", fd_, rc);
            // Is the operation complete?
            if (rc >= 0) {
                PROFILING_SCOPE_N("async_accept::try_run -- DONE");
                native_file_desc_t conn_fd = static_cast<native_file_desc_t>(rc);
                fcntl(conn_fd, F_SETFL, O_NONBLOCK);
                stdexec::set_value(std::move(recv_), connection{conn_fd});
                return true;
            }
            // Is the operation still in progress?
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return false;
            // General failure
            PROFILING_SCOPE_N("async_accept::try_run -- FAILURE");
            std::system_error err = std::error_code(errno, std::system_category());
            stdexec::set_error(std::move(recv_), err);
            return true;
        }
        auto set_stopped() noexcept -> void override { stdexec::set_stopped(std::move(recv_)); }

    public:
        oper(Recv&& recv, io_context* ctx, native_file_desc_t fd)
            : recv_(std::move(recv))
            , ctx_(ctx)
            , fd_(fd) {}

        friend void tag_invoke(stdexec::start_t, oper& self) noexcept {
            PROFILING_SCOPE_N("async_accept::start");
            try {
                self.ctx_->get_scheduler().add_io_oper(self.fd_, oper_type::read, &self);
            } catch (...) {
                std::system_error err = std::make_error_code(std::errc::operation_not_permitted);
                stdexec::set_error(std::move(self.recv_), err);
            }
        }
    };

    template <stdexec::receiver Recv>
    friend auto tag_invoke(stdexec::connect_t, async_accept_sender self, Recv&& recv)
            -> oper<std::decay_t<Recv>> {
        return {std::forward<Recv>(recv), self.ctx_, self.fd_};
    }
};
} // namespace detail

inline auto async_accept(io_context& ctx, const listening_socket& sock)
        -> detail::async_accept_sender {
    return {&ctx, sock.fd()};
}

} // namespace io
