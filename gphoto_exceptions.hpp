#pragma once

#include <string>
#include <stdexcept>
#include <boost/python.hpp>

class GPhotoException : public std::runtime_error {
  public:

  explicit GPhotoException(const std::string& msg) : std::runtime_error(msg) { }
};


void translateException(const std::runtime_error& x) {
  std::string message = std::string("GPhotoException: ") + x.what();
  PyErr_SetString(PyExc_UserWarning, message.c_str());
};
