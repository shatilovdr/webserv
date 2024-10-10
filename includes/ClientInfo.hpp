/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshatilo <dshatilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:38:49 by klukiano          #+#    #+#             */
/*   Updated: 2024/10/10 10:16:38 by dshatilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ClientInfo_HPP
#define ClientInfo_HPP

#include "HttpParser.hpp"
#include <poll.h>
#include <fstream>

class Socket;
class VirtualHost;

class ClientInfo
{
 public:
  ClientInfo() = default;
  // ClientInfo(int fd, Socket* sock);
  ClientInfo(const ClientInfo& other)             = delete;
  ClientInfo& operator=(const ClientInfo& other)  = delete;
  ~ClientInfo()                                   = default;

  void            InitInfo(int fd, Socket *sock);
  int             RecvRequest(pollfd& poll);
  HttpParser&     getParser();
  VirtualHost*    getVhost();
  int             getFd();
  bool            getIsSending();
  std::ifstream&  getGetfile();
  void            setIsSending(bool boolean);

private:
  int            fd_;
  Socket*        sock_ = nullptr;
  VirtualHost*   vhost_ = nullptr;
  HttpParser     parser_;
  std::ifstream  getfile_;
  bool           is_sending_chunks_ = false;
  bool           is_parsing_body_ = false;
};

#endif