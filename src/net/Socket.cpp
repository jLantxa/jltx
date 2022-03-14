/*
 * MIT License
 *
 * Copyright (c) 2020 Javier Lancha Vázquez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "net/Socket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.hpp"

static __attribute_used__ const char* LOG_TAG = "jltx::net::Socket";

namespace jltx {

namespace net {

Connection::Connection(int sockfd) : m_sockfd(sockfd) {}

Connection::~Connection() { Close(); }

ssize_t Connection::Send(void* buffer, std::size_t len) const {
  return send(m_sockfd, buffer, len, 0);
}

ssize_t Connection::Read(void* buffer, std::size_t len, int flags) const {
  return recv(m_sockfd, buffer, len, flags);
}

void Connection::Close() {
  setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (const void*)true,
             sizeof(int));
}

Socket::Socket(Domain domain, Type type)
    : m_domain(static_cast<int>(domain)), m_type(static_cast<int>(type)) {
  m_sockfd = socket(m_domain, m_type, 0);
}

ServerSocket::ServerSocket(Domain domain, Type type, uint16_t port)
    : Socket(domain, type) {
  ::jltx::debug::Log.d(LOG_TAG, "%s():", __func__);

  m_port = port;
  m_address.sin_family = static_cast<sa_family_t>(m_domain);
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons(m_port);

  int ret = bind(m_sockfd, (struct sockaddr*)&m_address, sizeof(m_address));
  if (ret < 0) {
    throw SocketException(SocketException::Action::BIND,
                          "Error binding server socket");
  }
  ::jltx::debug::Log.i(LOG_TAG, "%s(): Created server socket", __func__);
}

void ServerSocket::Listen() {
  int ret = listen(m_sockfd, 0);
  if (ret < 0) {
    throw SocketException(SocketException::Action::LISTEN,
                          "Server socket could not listen");
  }
}

Connection ServerSocket::Accept() {
  socklen_t socklen = sizeof(m_address);
  int newSockfd;

  static constexpr unsigned MAX_RETRIES = 10;
  unsigned int numTries = 0;

  do {
    newSockfd = accept(m_sockfd, (struct sockaddr*)&m_address, &socklen);
    numTries++;

    if (newSockfd >= 0) {
      Connection connection(newSockfd);
      ::jltx::debug::Log.i(LOG_TAG, "%s(): Accepted new socket", __func__);
      return connection;
    }
  } while (numTries < MAX_RETRIES);

  throw SocketException(SocketException::Action::ACCEPT,
                        "Server socket could not accept a connection");
}

ClientSocket::ClientSocket(Domain domain, Type type, std::string address,
                           uint16_t port)
    : Socket(domain, type) {
  ::jltx::debug::Log.d(LOG_TAG, "%s():", __func__);

  m_port = port;
  m_address.sin_family = static_cast<sa_family_t>(m_domain);
  inet_pton(AF_INET, address.c_str(), &m_address.sin_addr);
  m_address.sin_port = htons(m_port);

  ::jltx::debug::Log.i(LOG_TAG, "%s(): Created client socket", __func__);
}

void ClientSocket::Connect() {
  int ret = connect(m_sockfd, (struct sockaddr*)&m_address, sizeof(m_address));
  if (ret < 0) {
    throw SocketException(SocketException::Action::CONNECT,
                          "Error connecting client socket");
  }
}

}  // namespace net
}  // namespace jltx
