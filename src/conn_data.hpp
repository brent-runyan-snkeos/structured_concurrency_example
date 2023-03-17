#pragma once

#include "io/connection.hpp"
#include "io/io_context.hpp"
#include "exec/static_thread_pool.hpp"

//! Structure packing together important objects for a connection
struct conn_data {
    io::connection conn_;
    io::io_context& io_ctx_;
    exec::static_thread_pool& pool_;
};
