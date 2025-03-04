/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  dshatilo < dshatilo@student.hive.fi >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:08:10 by  dshatilo         #+#    #+#             */
/*   Updated: 2024/10/22 22:54:11 by  dshatilo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALHOST_HPP_
#define VIRTUALHOST_HPP_

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <netdb.h> 
#include <poll.h>
#include <vector>

#include "Location.hpp"
#include "HttpResponse.hpp"
#include "HttpParser.hpp"

class VirtualHost {
 private:
  using StringMap = std::map<std::string, std::string>;
  using LocationMap = std::map<std::string, Location>;
  using StringPair = std::pair<std::string, std::string>;

 public:
  VirtualHost(std::string& name,
              std::string& max_size,
              StringMap& errors,
              LocationMap& locations);
  VirtualHost(const VirtualHost& other)             = default; //Why delete doesn't work?
  VirtualHost(VirtualHost&& other)                  = default;
  VirtualHost& operator=(const VirtualHost& other)  = delete;
  VirtualHost& operator=(VirtualHost&& other)       = delete;

  ~VirtualHost() = default;

  std::string   ToString() const;
  size_t        getMaxBodySize() const;
  LocationMap&  getLocations();
  std::string   getErrorPage(const std::string& error) const;
  std::string   getName() const;

 private:
  static const StringMap& getDefaultErrorPages();

  std::string name_;
  StringMap   error_pages_;
  size_t      client_max_body_size_ = 1048576;
  LocationMap locations_;
};

#endif