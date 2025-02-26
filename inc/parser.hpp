#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstddef>
#include <limits>
#include <sstream>
#include <fstream>
#include "CDParserFile.hpp"
#include "CDLocation.hpp"
#include "CDServer.hpp"

#define DEFAULT_CONFIG_ParserFile "test/nginx/conf/requestTest2.conf"

enum methodsEnum{
    GET,
    POST,
    DELETE
};

enum ConfigType {
    SERVER,
    SERVER_NAME,
    LOCATION,
    REDIRECT,
    ERROR_PAGE,
    PORT,
    ROOT,
    INDEX,
    METHODS,
    AUTO_INDEX,
    MAX_BODY_SIZE,
    UPLOAD_PATH,
    BRACE_CLOSE,
    CGI,
    UNKNOWN
};

//UTILS
std::vector<std::string> splitString(const std::string& input, char delimiter);
unsigned long stringToUnsignedLong(const std::string& str);
short stringToShort(const std::string& str);


template <typename T>
std::string toString(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

#endif
