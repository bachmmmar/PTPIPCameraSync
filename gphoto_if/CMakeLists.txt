
set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/gphoto_helper.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/gphoto_if.cc
   PARENT_SCOPE
)
set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/gphoto_helper.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/gphoto_if.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/gphoto_exceptions.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/gphoto_python_exports.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/gphoto_typedef.hpp
   PARENT_SCOPE
)



