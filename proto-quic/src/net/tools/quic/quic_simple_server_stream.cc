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
// TODO(weiyu): change LargestStreamId to a class member
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
    MarkConsumed(iov.iov_len);
  }
  if (!sequencer()->IsClosed()) {
    sequencer()->SetUnblocked();
    return;
  }
  
  std::string message = body_;

    // JS
    if (id() < LargestStreamId){
      //JS:It means we got an old message, Log it to keep track of how many times this happens
      std::cout << "Received old stream!! " << id() << ", " << LargestStreamId << std::endl;
      std::ofstream logging_old_packets;
      logging_old_packets.open("/home/lca2/Desktop/Old_Messages.txt", std::ios_base::app);
      logging_old_packets << id() << std::endl;
      logging_old_packets.close();
    } else {
      //JS: update the value of LargestStreamId, because we encountered a stream id that
      //JS: is larger than the largest one seen so far
      LargestStreamId = id();
//      std::string message(static_cast<char *>(iov.iov_base));
      // spdy_session()->OnEphemeralMessageReceived(message);
    }

    //JS: split the message string into packet_number and timestamp
    std::string::size_type pos = message.find(":");
    // A complete client request has more than 100 characters, including "packet_number:timestamp:" and padding characters.
    if ((pos != std::string::npos) && (message.find(":", pos+1) != std::string::npos) && (message.length() >= 100)) {
      std::cout << "stoll packet_number: " << message.substr(0, pos) << std::endl;
      long long packet_number = std::stol(message.substr(0, pos));
      //JS: Use long long for high precision
      std::cout << "stoll sending_timestamp: " << message.substr(pos+1) << std::endl;
      long long sending_timestamp = std::stol(message.substr(pos+1));

      //JS: Get one way delay from client to server (in microseconds)
      long long current_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      long long delay = current_timestamp - sending_timestamp;
      std::cout << "ONE WAY DELAY " << packet_number << ": " << delay << std::endl;

      //JS: Write delays to output file, for future analysis
      std::ofstream logging_delay_client;
      logging_delay_client.open("/home/lca2/Desktop/delay_client_server.txt", std::ios_base::app);
      logging_delay_client << packet_number << ": " << delay << std::endl;
      logging_delay_client.close();


  SpdyHeaderBlock dummy_headers;
  dummy_headers[":status"] = "404";
//  long long current_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::string body = std::to_string(sending_timestamp) + ":" + std::to_string(current_timestamp); 
  SendHeadersAndBody(std::move(dummy_headers), body);

    } else {
       std::cout << "Ignored incomplete request: " << message << std::endl;
    }


  // If the sequencer is closed, then all the body, including the fin, has been
  // consumed.
  OnFinRead();

  if (write_side_closed() || fin_buffered()) {
    return;
  }



//  SendResponse();
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
  long current_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  SpdyHeaderBlock headers;
  headers[":status"] = "404";
  SendHeadersAndBody(std::move(headers), std::to_string(current_timestamp));
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
