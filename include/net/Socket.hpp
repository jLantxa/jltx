/*
 * Copyright (C) 2020  Javier Lancha Vázquez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _JLTX_INCLUDE_NET_SOCKET_HPP_
#define _JLTX_INCLUDE_NET_SOCKET_HPP_

#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdint>
#include <string>
#include <vector>

namespace jltx {

/** Networking classes and utilities */
namespace net {

/**
 * \brief An established connection with a socket
 */
class Connection {
 public:
  Connection() = default;
  Connection(int sockfd);
  virtual ~Connection();

  /** \brief Send a buffer of bytes
   * \param buffer A pointer to a buffer.
   * \param len The size of the buffer.
   * \returns Number of bytes sent.
   */
  virtual ssize_t Send(void* buffer, std::size_t len) const;

  /** \brief Read a buffer of bytes
   * \param buffer A pointer to a buffer to store the received data.
   * \param len The size of the buffer
   * \returns Number of bytes read.
   */
  virtual ssize_t Read(void* buffer, std::size_t len,
                       int flags = MSG_DONTWAIT) const;

  /**
   * \brief Close the socket.
   */
  void Close();

 protected:
  /** File descriptor for the socket */
  int m_sockfd;
};

/** \brief An exception class related to Socket errors */
class SocketException : public std::exception {
 public:
  /** Action that caused the exception */
  enum class Action {
    BIND,
    LISTEN,
    ACCEPT,
    CONNECT,
  };

  SocketException(Action action, const char* msg)
      : mAction(action), mMsg(msg) {}
  SocketException(Action action)
      : SocketException(action, "Socket exception") {}

  const char* what() const throw() override { return mMsg; }

  Action action() const { return mAction; }

 protected:
  const Action mAction;
  const char* mMsg;
};

/** \brief Base server socket class */
class Socket : public Connection {
 public:
  enum class Domain : int {
    LOCAL = AF_UNIX,
    IPv4 = AF_INET,
    IPv6 = AF_INET6,
  };

  enum class Type {
    STREAM = SOCK_STREAM,
    DATAGRAM = SOCK_DGRAM,
  };

  Socket(Domain domain, Type type);
  virtual ~Socket() = default;

 protected:
  int m_domain;
  int m_type;
  struct sockaddr_in m_address;
  uint16_t m_port;
};

/** \brief A server socket */
class ServerSocket : public Socket {
 public:
  /** \brief Construct a ServerSocket
   * \param domain IPv4, IPv6 or LOCAL
   * \param type Stream or datagram
   * \param port Port number
   */
  ServerSocket(Domain domain, Type type, uint16_t port);

  virtual ~ServerSocket() = default;

  /** \brief Listen for incoming connections. */
  void Listen();

  /** \brief Accept the next incoming connection request. */
  Connection Accept();
};

/** \brief A client socket */
class ClientSocket : public Socket {
 public:
  /** \brief Construct a ClientSocket
   * \param domain IPv4, IPv6 or LOCAL
   * \param type Stream or datagram
   * \param address IP Address
   * \param port Port number
   */
  ClientSocket(Domain domain, Type type, std::string address, uint16_t port);

  virtual ~ClientSocket() = default;

  /** Connect this socket to a server with the parameters specified in the
   * constructor*/
  void Connect();
};

}  // namespace net
}  // namespace jltx

#endif  // _JLTX_INCLUDE_TEXT_UTILS_HPP_
