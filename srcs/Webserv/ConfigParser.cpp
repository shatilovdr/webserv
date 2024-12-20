/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dshatilo <dshatilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:55:31 by klukiano          #+#    #+#             */
/*   Updated: 2024/11/04 16:43:05 by dshatilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "WebServ.hpp"

ConfigParser::ConfigParser(const std::string& conf) : conf_(conf) {
  logInfo("Opening configuration file");
}

int ConfigParser::ParseConfig(std::deque<Socket>& sockets) {
  if (!conf_.is_open()) {
    logError("Can't open configuration file");
    return 1;
  }

  std::stringstream ss;
  while (!conf_.eof()) {
    std::string line;
    std::getline(conf_, line);
    if (size_t pos = line.find('#'); pos != std::string::npos)
      line.resize(pos);
    ss << line << '\n';
  }

  while (!ss.eof()) {
    try {
      ParseServer(ss, sockets);
    } catch (std::string& error_token) {
      logError("Invalid input: \"", error_token, "\".");
      return 1;
    } catch (const char* e) {
      logError(e);
      return 1;
    }
  }
  if (sockets.empty()) {
    logError("Missing servers configurations");
    return 1;
  }
  return 0;
}

void ConfigParser::ParseServer(std::stringstream& ss,
                               std::deque<Socket> &sockets_) {
  std::string token;
  ss >> token;
  if (token == "")
    return;
  if (token != "server")
    throw token;
  ss >> token;
  if (token != "{")
    throw token;

  std::string listen;
  std::string server_name;
  std::string max_size;
  StringMap   errors;
  LocationMap locations;

  while (true) {
    ss >> token;
    if (token == "listen")
      ParseListen(listen, ss);
    else if (token == "server_name")
      ParseServerName(server_name, ss);
    else if (token == "client_max_body_size")
      ParseMaxBodySize(max_size, ss);
    else if (token == "error_page")
      ParseErrorPage(errors, ss);
    else if (token == "location")
      ParseLocation(locations, ss);
    else if (token == "}")
      break;
    else
      throw token;
  }
if (listen.empty() || locations.empty()) {
  throw "Incomplete server configuration";
}
auto it = std::find_if(sockets_.begin(), sockets_.end(), [&](Socket& obj) {
      return obj.getSocket() == listen;
  });
  if (it != sockets_.end())
    it->AddVirtualHost(server_name, max_size, errors, locations);
  else
    sockets_.push_back(Socket(listen, server_name, max_size, errors,
                              locations));
}

void ConfigParser::ParseListen(std::string& socket, std::stringstream& ss) {
  static std::regex format("((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\\.)"
                           "{3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]):"
                           "(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4]"
                           "[0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{1,3}|[0-9]);");
  ss >> socket;
  if (!std::regex_match(socket, format))
    throw "listen " + socket;
  socket.pop_back();
}

void ConfigParser::ParseServerName(std::string& name, std::stringstream& ss) {
  static std::regex format(R"(([a-z0-9-]{1,63}\.){1,124}com;)");

  std::string token;
  ss >> token;
  if (!std::regex_match(token, format) || token.size() > 253)
    throw "server_name " + token;
  token.pop_back();
  if (name.empty())
    name = token;
}

void ConfigParser::ParseMaxBodySize(std::string& max_size,
                                    std::stringstream& ss) {
  std::regex format("0;|((0|[1-9][0-9]{0,5}|1000000)K;)"
                    "|((0|[1-9][0-9]{0,2}|1000)M;)");

  std::string token;
  ss >> token;
  if (!std::regex_match(token, format))
    throw "client_max_body_size " + token;
  token.pop_back();
  if (max_size.empty())
    max_size = token;
}

void ConfigParser::ParseErrorPage(StringMap& errors, std::stringstream& ss) {
  static std::regex code_format("404|403|404|405|411|413|415|431|"
                                "500|501|502|504|505");
  static std::regex path_format("/([a-zA-Z0-9_-]*/)*[a-zA-Z0-9_-]+\\.html;");

  std::string code;
  std::string path;
  ss >> code;
  ss >> path;
  if (!std::regex_match(code, code_format)
      || !std::regex_match(path, path_format))
    throw "error_page " + code + " " + path;
  path.pop_back();
  if (!errors.contains(code))
    errors.emplace(code, path.data() + 1);
}

void ConfigParser::ParseLocation(LocationMap& locations,
                                 std::stringstream& ss) {
  static std::regex location_format("/[a-zA-Z0-9_-]*");

  std::string location;
  ss >> location;
  if (!std::regex_match(location, location_format))
    throw "location " + location;

  std::string methods;
  StringPair  redirection;
  std::string root;
  std::string autoindex;
  std::string index;
  std::string upload;

  std::string token;
  ss >> token;
  if (token != "{")
    throw token;
  while (true) {
    ss >> token;
    logDebug(token);
    if (token == "limit_except")
      ParseAllowedMethods(methods, ss);
    else if (token == "return")
      ParseRedirection(redirection, ss);
    else if (token == "root")
      ParseRoot(root, ss);
    else if (token == "autoindex")
      ParseAutoindex(autoindex, ss);
    else if (token == "index")
      ParseIndex(index, ss);
    else if (token == "upload")
      ParseUpload(upload, ss);
    else if (token == "}")
      break;
    else
      throw token;
  }
  if (root.empty())
    throw "Incomplete location configuration";
  if (!locations.contains(location))
    locations.emplace(
      location, Location(methods, redirection, root, autoindex, index, upload));
}

void ConfigParser::ParseAllowedMethods(std::string& methods,
                                       std::stringstream& ss) {
  static std::regex format("\\s*((GET|POST|DELETE)\\s+)*"
                                "(GET|POST|DELETE)\\s*"); 
  std::string line;
  std::getline(ss, line);
  if (!std::regex_match(line, format))
    throw "limit_except " + line;
  if (methods.empty())
    methods = line;
}

void ConfigParser::ParseRedirection(StringPair& redirection,
                                     std::stringstream& ss) {
  static std::regex code_format("30[0-478]");
  static std::regex path_format("(/([a-zA-Z0-9_-]*/)*[a-zA-Z0-9_-]+\\.html)|"
                                "((https?://)?(www\\.)?([a-zA-Z0-9_-]+\\.)+"
                                "[a-zA-Z]{2,});");
  std::string code;
  std::string path;
  ss >> code;
  ss >> path;
  if (!std::regex_match(code, code_format)
      || !std::regex_match(path, path_format))
    throw "return " + code + " " + path;
  path.pop_back();
  if (redirection.first.empty())
    redirection = {code, path};
}

void ConfigParser::ParseRoot(std::string& root, std::stringstream& ss) {
  static std::regex format("(/[a-zA-Z0-9_-]+)+;");

  std::string token;
  ss >> token;
  if (!std::regex_match(token, format))
    throw "root " + token;
  token.back() = '/';
  if (root.empty())
    root = token.substr(1);
}

void ConfigParser::ParseAutoindex(std::string& autoindex,
                                  std::stringstream& ss) {
  static std::regex format("(on|off);");

  std::string token;
  ss >> token;
  if (!std::regex_match(token, format))
    throw "autoindex " + token;
  token.pop_back();
  if (autoindex.empty())
    autoindex = token;
}

void ConfigParser::ParseIndex(std::string& index, std::stringstream& ss) {
  static std::regex format("([a-zA-Z0-9_-]+/)*[a-zA-Z0-9_-]+\\.html;");

  std::string token;
  ss >> token;
  if (!std::regex_match(token, format))
    throw "index " + token;
  token.pop_back();
  if (index.empty())
    index = token;
}

void ConfigParser::ParseUpload(std::string& upload, std::stringstream& ss) {
  static std::regex format("/[a-zA-Z0-9_-]+;");

  std::string token;
  ss >> token;
  if (!std::regex_match(token, format))
    throw "upload " + token;
  token.back() = '/';
  if (upload.empty())
    upload = token.substr(1);
}