#pragma once

#include "img_transform.hpp"
#include "conn_data.hpp"
#include "parsed_uri.hpp"

#include <exec/task.hpp>

namespace http_server {
struct http_request;
struct http_response;
} // namespace http_server

auto handle_blur(const conn_data& cdata, http_server::http_request&& req, parsed_uri puri)
        -> http_server::http_response;

auto handle_adaptthresh(const conn_data& cdata, http_server::http_request&& req, parsed_uri puri)
        -> http_server::http_response;

auto handle_reducecolors(const conn_data& cdata, http_server::http_request&& req, parsed_uri puri)
        -> http_server::http_response;

auto handle_cartoonify(const conn_data& cdata, http_server::http_request&& req, parsed_uri puri)
        -> exec::task<http_server::http_response>;

auto handle_oilpainting(const conn_data& cdata, http_server::http_request&& req, parsed_uri puri)
        -> http_server::http_response;

auto handle_contourpaint(const conn_data& cdata, http_server::http_request&& req, parsed_uri puri)
        -> exec::task<http_server::http_response>;
