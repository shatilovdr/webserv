/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshatilo <dshatilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:16:12 by vsavolai          #+#    #+#             */
/*   Updated: 2024/10/10 11:25:38 by dshatilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPARSER_HPP_
# define HTTPPARSER_HPP_

# include <iostream>
# include <map>
# include <sstream>
# include <string>
# include <algorithm>
# include <filesystem>
# include <unistd.h>
# include <fcntl.h>
# include <vector>
# include <array>
# include "VirtualHost.hpp"

#define MAXBYTES 8192

class VirtualHost;

class HttpParser {
 public:
  HttpParser() = default;
  HttpParser(const std::string buffer);
  ~HttpParser() = default;

  bool        ParseHeader(const std::string& buffer);
  int         WriteBody(VirtualHost* vhost,  std::vector<char>& buffer,
                        int bytesIn);
  bool        UnChunkBody(std::vector<char>& buf);
  void        AppendBody(std::vector<char> buffer, int bytesIn);
  bool        IsBodySizeValid(VirtualHost* vhost) ;
  std::string getHost() const;
  std::string getMethod() const;
  std::string getResourcePath() const;
  int         getErrorCode() const;
  void        setErrorCode(int error) ;

 private:
  int                                 error_code_ = 200;
  size_t                              content_length_ = 0;
  std::string                         method_;
  std::string                         resource_path_;
  std::string                         query_string_;
  std::string                         http_version_;
  std::vector<char>                   request_body_;
  std::map<std::string, std::string>  headers_ = {};
  bool                                is_chunked_;
};

#endif