// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/tools/quic/quic_simple_server_stream.h"

#include <list>
#include <utility>

//JS
#include <iostream>
#include <chrono>
#include <fstream>

#include "net/quic/core/quic_spdy_stream.h"
#include "net/quic/core/spdy_utils.h"
#include "net/quic/platform/api/quic_bug_tracker.h"
#include "net/quic/platform/api/quic_flags.h"
#include "net/quic/platform/api/quic_logging.h"
#include "net/quic/platform/api/quic_map_util.h"
#include "net/quic/platform/api/quic_text_utils.h"
#include "net/spdy/core/spdy_protocol.h"
#include "net/tools/quic/quic_http_response_cache.h"
#include "net/tools/quic/quic_simple_server_session.h"

using std::string;

//JS
using namespace std::chrono;

//JS: Initialize LargestStreamId to 0 (as expected)
unsigned int net::QuicSimpleServerStream::LargestStreamId = 0;

namespace net {

QuicSimpleServerStream::QuicSimpleServerStream(
    QuicStreamId id,
    QuicSpdySession* session,
    QuicHttpResponseCache* response_cache)
    : QuicSpdyServerStreamBase(id, session),
      content_length_(-1),
      response_cache_(response_cache) {}

QuicSimpleServerStream::~QuicSimpleServerStream() {}

void QuicSimpleServerStream::OnInitialHeadersComplete(
    bool fin,
    size_t frame_len,
    const QuicHeaderList& header_list) {
  QuicSpdyStream::OnInitialHeadersComplete(fin, frame_len, header_list);
  if (!SpdyUtils::CopyAndValidateHeaders(header_list, &content_length_,
                                         &request_headers_)) {
    QUIC_DVLOG(1) << "Invalid headers";
    SendErrorResponse();
  }
  ConsumeHeaderList();
}

void QuicSimpleServerStream::OnTrailingHeadersComplete(
    bool fin,
    size_t frame_len,
    const QuicHeaderList& header_list) {
  QUIC_BUG << "Server does not support receiving Trailers.";
  SendErrorResponse();
}

void QuicSimpleServerStream::OnDataAvailable() {
  while (HasBytesToRead()) {
    struct iovec iov;
    if (GetReadableRegions(&iov, 1) == 0) {
      // No more data to read.
      break;
    }

    QUIC_DVLOG(1) << "Stream " << id() << " processed " << iov.iov_len
                  << " bytes.";
    body_.append(static_cast<char*>(iov.iov_base), iov.iov_len);

    //JS
    if (id() < LargestStreamId){
      //JS:It means we got an old message, Log it to keep track of how many times this happens
      std::cout << "Received old stream!! " << std::to_string(id()) + ", " + std::to_string(LargestStreamId) << std::endl;
      std::ofstream logging_old_packets;
      logging_old_packets.open("/home/lca2/Desktop/Old_Messages.txt", std::ios_base::app);
      logging_old_packets << id() << std::endl;
      logging_old_packets.close();
    }

    else {
      //JS: update the value of LargestStreamId, because we encountered a stream id that
      //JS: is larger than the largest one seen so far
      LargestStreamId = id();
    }

    //JS: Get the body of the request of the client, it should be of the following format: "packet_number:timestamp"
    string client_request = std::string(static_cast<char*>(iov.iov_base));

    //JS: split the client_request string into packet_number and timestamp
    int pos = client_request.find(":");
    long packet_number = std::stol(client_request.substr(0, pos));
    //JS: Use long for high precision
    //long latest_response_timestamp = std::stol(static_cast<char*>(iov.iov_base));
    long latest_response_timestamp = std::stol(client_request.substr(pos+1));

    //JS: Get one way delay from client to server (in microseconds)
    long current_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long delay = current_timestamp - latest_response_timestamp;
    std::cout << "ONE WAY DELAY " + std::to_string(packet_number) + ": "<< delay << std::endl;

    //JS: Write delays to output file, for future analysis
    std::ofstream logging_delay_client;
    logging_delay_client.open("/home/lca2/Desktop/delay_client_server.txt", std::ios_base::app);
    logging_delay_client << std::to_string(packet_number) + ": " + std::to_string(delay)<< std::endl;
    logging_delay_client.close();

    if (content_length_ >= 0 &&
        body_.size() > static_cast<uint64_t>(content_length_)) {
      QUIC_DVLOG(1) << "Body size (" << body_.size() << ") > content length ("
                    << content_length_ << ").";
      SendErrorResponse();
      return;
    }
    MarkConsumed(iov.iov_len);
  }
  if (!sequencer()->IsClosed()) {
    sequencer()->SetUnblocked();
    return;
  }

  // If the sequencer is closed, then all the body, including the fin, has been
  // consumed.
  OnFinRead();

  if (write_side_closed() || fin_buffered()) {
    return;
  }

  SendResponse();
}

void QuicSimpleServerStream::PushResponse(
    SpdyHeaderBlock push_request_headers) {
  if (id() % 2 != 0) {
    QUIC_BUG << "Client initiated stream shouldn't be used as promised stream.";
    return;
  }
  // Change the stream state to emulate a client request.
  request_headers_ = std::move(push_request_headers);
  content_length_ = 0;
  QUIC_DVLOG(1) << "Stream " << id()
                << " ready to receive server push response.";

  // Set as if stream decompresed the headers and received fin.
  QuicSpdyStream::OnInitialHeadersComplete(/*fin=*/true, 0, QuicHeaderList());
}

void QuicSimpleServerStream::SendResponse() {
  if (request_headers_.empty()) {
    QUIC_DVLOG(1) << "Request headers empty.";
    SendErrorResponse();
    return;
  }

  if (content_length_ > 0 &&
      static_cast<uint64_t>(content_length_) != body_.size()) {
    QUIC_DVLOG(1) << "Content length (" << content_length_ << ") != body size ("
                  << body_.size() << ").";
    SendErrorResponse();
    return;
  }

  if (!QuicContainsKey(request_headers_, ":authority") ||
      !QuicContainsKey(request_headers_, ":path")) {
    QUIC_DVLOG(1) << "Request headers do not contain :authority or :path.";
    SendErrorResponse();
    return;
  }

  // Find response in cache. If not found, send error response.

  //JS
  QuicHttpResponseCache::Response* response = nullptr;
  auto authority = request_headers_.find(":authority");
  auto path = request_headers_.find(":path");
  if (authority != request_headers_.end() && path != request_headers_.end()) {
    response = response_cache_->GetResponse(authority->second, path->second);

    //JS: Modify the response, fetched from the cache, on the fly
    long current_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    response->set_body(QuicStringPiece(std::to_string(current_timestamp)));
    std::cout << "RESPONSE: " << current_timestamp << std::endl;

  }
  if (response == nullptr) {
    QUIC_DVLOG(1) << "Response not found in cache.";
    SendNotFoundResponse();
    return;
  }

  if (response->response_type() == QuicHttpResponseCache::CLOSE_CONNECTION) {
    QUIC_DVLOG(1) << "Special response: closing connection.";
    CloseConnectionWithDetails(QUIC_NO_ERROR, "Toy server forcing close");
    return;
  }

  if (response->response_type() == QuicHttpResponseCache::IGNORE_REQUEST) {
    QUIC_DVLOG(1) << "Special response: ignoring request.";
    return;
  }

  // Examing response status, if it was not pure integer as typical h2
  // response status, send error response. Notice that
  // QuicHttpResponseCache push urls are strictly authority + path only,
  // scheme is not included (see |QuicHttpResponseCache::GetKey()|).
  string request_url = request_headers_[":authority"].as_string() +
                       request_headers_[":path"].as_string();
  int response_code;
  const SpdyHeaderBlock& response_headers = response->headers();
  if (!ParseHeaderStatusCode(response_headers, &response_code)) {
    auto status = response_headers.find(":status");
    if (status == response_headers.end()) {
      QUIC_LOG(WARNING)
          << ":status not present in response from cache for request "
          << request_url;
    } else {
      QUIC_LOG(WARNING) << "Illegal (non-integer) response :status from cache: "
                        << status->second << " for request " << request_url;
    }
    SendErrorResponse();
    return;
  }

  if (id() % 2 == 0) {
    // A server initiated stream is only used for a server push response,
    // and only 200 and 30X response codes are supported for server push.
    // This behavior mirrors the HTTP/2 implementation.
    bool is_redirection = response_code / 100 == 3;
    if (response_code != 200 && !is_redirection) {
      QUIC_LOG(WARNING) << "Response to server push request " << request_url
                        << " result in response code " << response_code;
      Reset(QUIC_STREAM_CANCELLED);
      return;
    }
  }
  std::list<QuicHttpResponseCache::ServerPushInfo> resources =
      response_cache_->GetServerPushResources(request_url);
  QUIC_DVLOG(1) << "Stream " << id() << " found " << resources.size()
                << " push resources.";

  if (!resources.empty()) {
    QuicSimpleServerSession* session =
        static_cast<QuicSimpleServerSession*>(spdy_session());
    session->PromisePushResources(request_url, resources, id(),
                                  request_headers_);
  }

  QUIC_DVLOG(1) << "Stream " << id() << " sending response.";
  SendHeadersAndBodyAndTrailers(response->headers().Clone(), response->body(),
                                response->trailers().Clone());
}

void QuicSimpleServerStream::SendNotFoundResponse() {
  QUIC_DVLOG(1) << "Stream " << id() << " sending not found response.";
  SpdyHeaderBlock headers;
  headers[":status"] = "404";
  headers["content-length"] =
      QuicTextUtils::Uint64ToString(strlen(kNotFoundResponseBody));
  SendHeadersAndBody(std::move(headers), kNotFoundResponseBody);
}

void QuicSimpleServerStream::SendErrorResponse() {
  QUIC_DVLOG(1) << "Stream " << id() << " sending error response.";
  SpdyHeaderBlock headers;
  headers[":status"] = "500";
  headers["content-length"] =
      QuicTextUtils::Uint64ToString(strlen(kErrorResponseBody));
  SendHeadersAndBody(std::move(headers), kErrorResponseBody);
}

void QuicSimpleServerStream::SendHeadersAndBody(
    SpdyHeaderBlock response_headers,
    QuicStringPiece body) {
  SendHeadersAndBodyAndTrailers(std::move(response_headers), body,
                                SpdyHeaderBlock());
}

void QuicSimpleServerStream::SendHeadersAndBodyAndTrailers(
    SpdyHeaderBlock response_headers,
    QuicStringPiece body,
    SpdyHeaderBlock response_trailers) {
  // Send the headers, with a FIN if there's nothing else to send.
  bool send_fin = (body.empty() && response_trailers.empty());
  QUIC_DLOG(INFO) << "Stream " << id() << " writing headers (fin = " << send_fin
                  << ") : " << response_headers.DebugString();
  WriteHeaders(std::move(response_headers), send_fin, nullptr);

  //JS
  std::cout << "STREAM ID:" << id() << std::endl;
  //spdy_session()->CloseStream_ServerSide(id() - 2);

  if (send_fin) {
    // Nothing else to send.
    return;
  }

  // Send the body, with a FIN if there's no trailers to send.
  send_fin = response_trailers.empty();
  QUIC_DLOG(INFO) << "Stream " << id() << " writing body (fin = " << send_fin
                  << ") with size: " << body.size();
  if (!body.empty() || send_fin) {
    WriteOrBufferData(body, send_fin, nullptr);
  }
  if (send_fin) {
    // Nothing else to send.
    return;
  }

  // Send the trailers. A FIN is always sent with trailers.
  QUIC_DLOG(INFO) << "Stream " << id() << " writing trailers (fin = true): "
                  << response_trailers.DebugString();
  WriteTrailers(std::move(response_trailers), nullptr);
}

const char* const QuicSimpleServerStream::kErrorResponseBody = "bad";
const char* const QuicSimpleServerStream::kNotFoundResponseBody =
    "file not found";

}  // namespace net
