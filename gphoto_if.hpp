#pragma once

extern "C" {
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>
}
#include "gphoto_typedef.hpp"


class GPhotoIf {

public:
  GPhotoIf(std::string model, std::string ip, bool debug);
  ~GPhotoIf();

  std::string getSummary();

  StringList getFilenamesFromFolder(std::string folder);
  StringList getFoldernamesFromFolder(std::string folder);
  
private:
  std::string ip_;
  std::string model_;
  
  Camera* camera_;
  GPContext *context_;
  GPPortInfoList *port_info_list_;
  
  void openCamera();

};
