#pragma once

#include "io/io_context.hpp"
#include "io/connection.hpp"
#include "io/out_buffer.hpp"
#include <profiling.hpp>

namespace sys {
#include <sys/socket.h>
}
namespace io {

namespace detail {

struct async_read_sender {
    io_context* ctx_;
    native_file_desc_t fd_;
    out_buffer buf_;

    using completion_signatures = stdexec::completion_signatures< //
            stdexec::set_value_t(std::size_t),                    //
            stdexec::set_error_t(std::system_error),              //
            stdexec::set_stopped_t()>;

    template <stdexec::receiver Recv>
    class oper : oper_body_base {
        Recv recv_;
        io_context* ctx_;
        native_file_desc_t fd_;
        out_buffer buf_;

        auto try_run() noexcept -> bool override {
            PROFILING_SCOPE_N("async_read::try_run");
            int rc = sys::recv(fd_, buf_.data(), buf_.size(), sys::MSG_DONTWAIT);
            PROFILING_SET_TEXT_FMT(32, "fd=%d => %d", fd_, rc);
            // Is the operation complete?
            if (rc >= 0) {
                std::size_t num_received = static_cast<std::size_t>(rc);
                stdexec::set_value(std::move(recv_), num_received);
                return true;
            }
            // Is the operation still in progress?
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                return false;
            // General failure
            std::system_error err = std::error_code(errno, std::system_category());
            stdexec::set_error(std::move(recv_), err);
            return true;
        }
        auto set_stopped() noexcept -> void override { stdexec::set_stopped(std::move(recv_)); }

    public:
        oper(Recv&& recv, io_context* ctx, native_file_desc_t fd, out_buffer buf)
            : recv_(std::move(recv))
            , ctx_(ctx)
            , fd_(fd)
            , buf_(buf) {}

        friend void tag_invoke(stdexec::start_t, oper& self) noexcept {
            PROFILING_SCOPE_N("async_read::start");
            try {
                self.ctx_->get_scheduler().add_io_oper(self.fd_, oper_type::read, &self);
            } catch (...) {
                std::system_error err = std::make_error_code(std::errc::operation_not_permitted);
                stdexec::set_error(std::move(self.recv_), err);
            }
        }
    };

    template <stdexec::receiver Recv>
    friend auto tag_invoke(stdexec::connect_t, async_read_sender&& self, Recv&& recv)
            -> oper<std::decay_t<Recv>> {
        return {std::forward<Recv>(recv), self.ctx_, self.fd_, self.buf_};
    }
};
} // namespace detail

inline auto async_read(io_context& ctx, const connection& c, out_buffer buf)
        -> detail::async_read_sender {
    return {&ctx, c.fd(), buf};
}

} // namespace io
