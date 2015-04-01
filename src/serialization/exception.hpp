#ifndef SERIALIZATION_EXCEPTION_HPP_INCLUDED
#define SERIALIZATION_EXCEPTION_HPP_INCLUDED

#include <stdexcept>

class SerializationException: public std::runtime_error
{
public:
  SerializationException(const std::string& str):
    std::runtime_error(str) {}
};


#endif /* SERIALIZATION_EXCEPTION_HPP_INCLUDED */
