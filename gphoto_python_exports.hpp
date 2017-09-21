#pragma once

#include <boost/python.hpp>
#include "gphoto_if.hpp"
#include "gphoto_exceptions.hpp"

using namespace boost::python;
BOOST_PYTHON_MODULE(gphoto_if)
{
  register_exception_translator<GPhotoException>(translateException);
  
  class_<GPhotoIf>("GPhotoIf", init<std::string>())
        .def("greet", &GPhotoIf::greet)
        .def("set", &GPhotoIf::set)
    ;
}

