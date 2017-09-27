#pragma once

#include <stdexcept>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "gphoto_if.hpp"
#include "gphoto_exceptions.hpp"



void translateException(const std::runtime_error& x) {
  std::string message = std::string("GPhotoException: ") + x.what();
  PyErr_SetString(PyExc_UserWarning, message.c_str());
};


using namespace boost::python;
BOOST_PYTHON_MODULE(gphoto_if)
{
  register_exception_translator<GPhotoException>(translateException);

  class_<StringList>("StringList")
        .def(vector_indexing_suite<StringList>() );
  
  class_<GPhotoIf>("GPhotoIf", init<std::string, std::string, bool>())
        .def("getSummary", &GPhotoIf::getSummary)
        .def("getFilenamesFromFolder", &GPhotoIf::getFilenamesFromFolder)
        .def("getFoldernamesFromFolder", &GPhotoIf::getFoldernamesFromFolder)
        .def("downloadFile", &GPhotoIf::downloadFile)
    ;
}

