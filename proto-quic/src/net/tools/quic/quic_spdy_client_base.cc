// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//JS
#include <iostream>
#include <chrono>
#include <fstream>

#include "net/tools/quic/quic_spdy_client_base.h"

#include "net/quic/core/crypto/quic_random.h"
#include "net/quic/core/quic_server_id.h"
#include "net/quic/core/spdy_utils.h"
#include "net/quic/platform/api/quic_flags.h"
#include "net/quic/platform/api/quic_logging.h"
#include "net/quic/platform/api/quic_ptr_util.h"
#include "net/quic/platform/api/quic_text_utils.h"

using base::StringToInt;
using std::string;

//JS
using namespace std::chrono;

namespace net {

void QuicSpdyClientBase::ClientQuicDataToResend::Resend() {
  client_->SendRequest(*headers_, body_, fin_);
  headers_ = nullptr;
}

QuicSpdyClientBase::QuicDataToResend::QuicDataToResend(
    std::unique_ptr<SpdyHeaderBlock> headers,
    QuicStringPiece body,
    bool fin)
    : headers_(std::move(headers)), body_(body), fin_(fin) {}

QuicSpdyClientBase::QuicDataToResend::~QuicDataToResend() {}

QuicSpdyClientBase::QuicSpdyClientBase(
    const QuicServerId& server_id,
    const QuicVersionVector& supported_versions,
    const QuicConfig& config,
    QuicConnectionHelperInterface* helper,
    QuicAlarmFactory* alarm_factory,
    std::unique_ptr<NetworkHelper> network_helper,
    std::unique_ptr<ProofVerifier> proof_verifier)
    : QuicClientBase(server_id,
                     supported_versions,
                     config,
                     helper,
                     alarm_factory,
                     std::move(network_helper),
                     std::move(proof_verifier)),
      store_response_(false),
      latest_response_code_(-1),
      latest_ephemeral_stream_(nullptr) {}

QuicSpdyClientBase::~QuicSpdyClientBase() {
  // We own the push promise index. We need to explicitly kill
  // the session before the push promise index goes out of scope.
  ResetSession();
}

QuicSpdyClientSession* QuicSpdyClientBase::client_session() {
  return static_cast<QuicSpdyClientSession*>(QuicClientBase::session());
}

void QuicSpdyClientBase::InitializeSession() {
  client_session()->Initialize();
  client_session()->CryptoConnect();
}

void QuicSpdyClientBase::OnClose(QuicSpdyStream* stream) {

  DCHECK(stream != nullptr);

  QuicSpdyClientStream* client_stream =
      static_cast<QuicSpdyClientStream*>(stream);

  const SpdyHeaderBlock& response_headers = client_stream->response_headers();
  if (response_listener_ != nullptr) {
    response_listener_->OnCompleteResponse(stream->id(), response_headers,
                                           client_stream->data());
  }

  // Store response headers and body.
  if (store_response_) {
    auto status = response_headers.find(":status");
    if (status == response_headers.end() ||
        !QuicTextUtils::StringToInt(status->second, &latest_response_code_)) {
      QUIC_LOG(ERROR) << "Invalid response headers";
    }
    latest_response_headers_ = response_headers.DebugString();
    preliminary_response_headers_ =
        client_stream->preliminary_headers().DebugString();
    latest_response_header_block_ = response_headers.Clone();
    latest_response_body_ = client_stream->data();

    //JS
    if (latest_response_body_ != ""){
      long long sending_timestamp = std::stoll(latest_response_body_);
      std::string::size_type pos = latest_response_body_.find(':');
      //JS: Use long for higher precision
      long latest_response_timestamp = std::stol(latest_response_body_.substr(pos + 1));

      //JS: Get one way delay from server to client (in microseconds)
      long current_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      long delay = current_timestamp - latest_response_timestamp;
      std::cout << "ONE WAY DELAY " + std::to_string(stream->id()) + ": "<< delay << std::endl;

      //JS: Extract the packet number from the stream id
      //Since packet 1 corresponds to stream id 5, packet 2 to stream id 7, 3 to 9 ...
      //then, packet_number = (stream_id - 5)/2 + 1
      long packet_number = (stream->id() - 5)/2 + 1;

      //JS: Write delays to output file, for future analysis
      std::ofstream logging_delay_server;
      logging_delay_server.open("/home/lca2/Desktop/delay_server_client.txt", std::ios_base::app);
      logging_delay_server << packet_number << ": " << delay << std::endl;
      logging_delay_server.close();

      std::cout << "Request sent: " << sending_timestamp << "   Response sent: " << latest_response_timestamp << "   Response received: " << current_timestamp << std::endl;
      std::cout << "RTT: " << current_timestamp - sending_timestamp;
      if (current_timestamp - sending_timestamp > 100000) {
        std::cout << "     !!!!!!!!!!!!";
      }
      std::cout << std::endl;
      std::cout << "client->server: " << latest_response_timestamp - sending_timestamp << std::endl;
      std::cout << "server->client: " << current_timestamp - latest_response_timestamp << std::endl;


    if (latest_response_timestamp > highest_timestamp){
      highest_timestamp = latest_response_timestamp;
    } else if (latest_response_timestamp < highest_timestamp){
      latest_response_body_ = std::to_string(highest_timestamp);
      std::cout << "Choose previous response!" << std::endl;
      std::cout << std::to_string(stream->id()) + " OLD: " << latest_response_timestamp << std::endl;
      std::cout << std::to_string(stream->id()) + " NEW: " << latest_response_body_ << std::endl;
    }

    //JS: print response, along with stream id to see which stream closed now
    latest_response_body_ = std::to_string(stream->id()) + " " + latest_response_body_;
    std::cout << "Final Response: " << latest_response_body_ << std::endl;
  }

    latest_response_trailers_ =
        client_stream->received_trailers().DebugString();
  }
}

std::unique_ptr<QuicSession> QuicSpdyClientBase::CreateQuicClientSession(
    QuicConnection* connection) {
  return QuicMakeUnique<QuicSpdyClientSession>(*config(), connection,
                                               server_id(), crypto_config(),
                                               &push_promise_index_);
}

void QuicSpdyClientBase::SendEphemeralMessage(const std::string &message, int deadline_in_microsecond) {
  /*
  long long start_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  // Cancel the last latest stream if still open, to stop retransmitting old ephemeral messages
  if ((latest_ephemeral_stream_ != nullptr) && (!latest_ephemeral_stream_->IsClosed())) {
    std::cout << "Cancelling stream " << latest_ephemeral_stream_->id() << std::endl;
    client_session()->StopRetransmissions(latest_ephemeral_stream_->id());
    latest_ephemeral_stream_->Reset(QUIC_STREAM_CANCELLED);
  }
  */
  SpdyHeaderBlock dummy_header;
  SendRequest(dummy_header, message, /*fin=*/true);
  /*
  if (deadline_in_microsecond < 0) {
    return;
  }
  // If deadline specified, wait until the deadline and cancel current stream if still open
  while(true){
    WaitForEvents();
    long long current_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (current_timestamp - start_timestamp >= deadline_in_microsecond){
      break;
    }
  }
  if ((latest_ephemeral_stream_ != nullptr) && (!latest_ephemeral_stream_->IsClosed())) {
    std::cout << "Cancelling stream " << latest_ephemeral_stream_->id() << std::endl;
    client_session()->StopRetransmissions(latest_ephemeral_stream_->id());
    latest_ephemeral_stream_->Reset(QUIC_STREAM_CANCELLED);
  }
  */
}

void QuicSpdyClientBase::SendRequest(const SpdyHeaderBlock& headers,
                                     QuicStringPiece body,
                                     bool fin) {
  QuicClientPushPromiseIndex::TryHandle* handle;
  QuicAsyncStatus rv = push_promise_index()->Try(headers, this, &handle);
  if (rv == QUIC_SUCCESS)
    return;

  if (rv == QUIC_PENDING) {
    // May need to retry request if asynchronous rendezvous fails.
    AddPromiseDataToResend(headers, body, fin);
    return;
  }

  QuicSpdyClientStream* stream = CreateClientStream();
  if (stream == nullptr) {
    QUIC_BUG << "stream creation failed!";
    return;
  }

  //JS: Client uses new stream for every request (fin is set to true when sending any request)
  std::cout << "STREAM ID:" << stream->id() << std::endl;
  latest_ephemeral_stream_ = stream;
  stream->SendRequest(headers.Clone(), body, fin);

  // Record this in case we need to resend.
  MaybeAddDataToResend(headers, body, fin);
}

void QuicSpdyClientBase::SendRequestAndWaitForResponse(
    const SpdyHeaderBlock& headers,
    QuicStringPiece body,
    bool fin) {
  SendRequest(headers, body, fin);
}

void QuicSpdyClientBase::SendRequestsAndWaitForResponse(
    const std::vector<string>& url_list) {
  for (size_t i = 0; i < url_list.size(); ++i) {
    SpdyHeaderBlock headers;
    if (!SpdyUtils::PopulateHeaderBlockFromUrl(url_list[i], &headers)) {
      QUIC_BUG << "Unable to create request";
      continue;
    }
    SendRequest(headers, "", true);
  }
  while (WaitForEvents()) {
  }
}

QuicSpdyClientStream* QuicSpdyClientBase::CreateClientStream() {
  if (!connected()) {
    return nullptr;
  }

  auto* stream = static_cast<QuicSpdyClientStream*>(
      FLAGS_quic_reloadable_flag_quic_refactor_stream_creation
          ? client_session()->MaybeCreateOutgoingDynamicStream(kDefaultPriority)
          : client_session()->CreateOutgoingDynamicStream(kDefaultPriority));
  if (stream) {
    stream->set_visitor(this);
  }
  return stream;
}

int QuicSpdyClientBase::GetNumSentClientHellosFromSession() {
  return client_session()->GetNumSentClientHellos();
}

int QuicSpdyClientBase::GetNumReceivedServerConfigUpdatesFromSession() {
  return client_session()->GetNumReceivedServerConfigUpdates();
}

void QuicSpdyClientBase::MaybeAddDataToResend(const SpdyHeaderBlock& headers,
                                              QuicStringPiece body,
                                              bool fin) {
  if (!FLAGS_quic_reloadable_flag_enable_quic_stateless_reject_support) {
    return;
  }

  if (client_session()->IsCryptoHandshakeConfirmed()) {
    // The handshake is confirmed.  No need to continue saving requests to
    // resend.
    data_to_resend_on_connect_.clear();
    return;
  }

  // The handshake is not confirmed.  Push the data onto the queue of data to
  // resend if statelessly rejected.
  std::unique_ptr<SpdyHeaderBlock> new_headers(
      new SpdyHeaderBlock(headers.Clone()));
  std::unique_ptr<QuicDataToResend> data_to_resend(
      new ClientQuicDataToResend(std::move(new_headers), body, fin, this));
  MaybeAddQuicDataToResend(std::move(data_to_resend));
}

void QuicSpdyClientBase::MaybeAddQuicDataToResend(
    std::unique_ptr<QuicDataToResend> data_to_resend) {
  data_to_resend_on_connect_.push_back(std::move(data_to_resend));
}

void QuicSpdyClientBase::ClearDataToResend() {
  data_to_resend_on_connect_.clear();
}

void QuicSpdyClientBase::ResendSavedData() {
  // Calling Resend will re-enqueue the data, so swap out
  //  data_to_resend_on_connect_ before iterating.
  std::vector<std::unique_ptr<QuicDataToResend>> old_data;
  old_data.swap(data_to_resend_on_connect_);
  for (const auto& data : old_data) {
    data->Resend();
  }
}

void QuicSpdyClientBase::AddPromiseDataToResend(const SpdyHeaderBlock& headers,
                                                QuicStringPiece body,
                                                bool fin) {
  std::unique_ptr<SpdyHeaderBlock> new_headers(
      new SpdyHeaderBlock(headers.Clone()));
  push_promise_data_to_resend_.reset(
      new ClientQuicDataToResend(std::move(new_headers), body, fin, this));
}

bool QuicSpdyClientBase::CheckVary(const SpdyHeaderBlock& client_request,
                                   const SpdyHeaderBlock& promise_request,
                                   const SpdyHeaderBlock& promise_response) {
  return true;
}

void QuicSpdyClientBase::OnRendezvousResult(QuicSpdyStream* stream) {
  std::unique_ptr<ClientQuicDataToResend> data_to_resend =
      std::move(push_promise_data_to_resend_);
  if (stream) {
    stream->set_visitor(this);
    stream->OnDataAvailable();
  } else if (data_to_resend) {
    data_to_resend->Resend();
  }
}

size_t QuicSpdyClientBase::latest_response_code() const {
  QUIC_BUG_IF(!store_response_) << "Response not stored!";
  return latest_response_code_;
}

const string& QuicSpdyClientBase::latest_response_headers() const {
  QUIC_BUG_IF(!store_response_) << "Response not stored!";
  return latest_response_headers_;
}

const string& QuicSpdyClientBase::preliminary_response_headers() const {
  QUIC_BUG_IF(!store_response_) << "Response not stored!";
  return preliminary_response_headers_;
}

const SpdyHeaderBlock& QuicSpdyClientBase::latest_response_header_block()
    const {
  QUIC_BUG_IF(!store_response_) << "Response not stored!";
  return latest_response_header_block_;
}

const string& QuicSpdyClientBase::latest_response_body() const {
  QUIC_BUG_IF(!store_response_) << "Response not stored!";
  return latest_response_body_;
}

const string& QuicSpdyClientBase::latest_response_trailers() const {
  QUIC_BUG_IF(!store_response_) << "Response not stored!";
  return latest_response_trailers_;
}

}  // namespace net
