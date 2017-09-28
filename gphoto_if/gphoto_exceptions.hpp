#pragma once

#include <string>
#include <stdexcept>

class GPhotoException : public std::runtime_error {
  public:

  explicit GPhotoException(const std::string& msg) : std::runtime_error(msg) { }
};

