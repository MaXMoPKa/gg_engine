#include "src/include/exception.hpp"
#include <sstream>

gg::Exception::Exception(int line, const char* file) noexcept
    : line{line}
    , file{file}
{}

const char* gg::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << std::endl << getOriginString();
    what_buffer = oss.str();
    return what_buffer.c_str();
}

const char* gg::Exception::getType() const noexcept
{
    return "GG Engine Exception";
}

int gg::Exception::getLine() const noexcept
{
    return line;
}

const std::string& gg::Exception::getFile() const noexcept
{
    return file;
}

std::string gg::Exception::getOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file << std::endl << "[Line] " << line;
    return oss.str();
}
