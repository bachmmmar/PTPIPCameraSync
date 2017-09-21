#pragma once

extern "C" {
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>
  //#include <gphoto2/gphoto2-setting.h>
  //#include <gphoto2/gphoto2-filesys.h>
}

class GPhotoIf {

public:
  GPhotoIf(std::string ip);
  ~GPhotoIf();

  void set(std::string msg) { ip_ = msg; }
  std::string greet() { return "hello,world"; }

private:
  std::string ip_;
  
  Camera* camera_;
  GPContext *context_;
  GPPortInfoList *port_info_list_;
  
  void openCamera();

};
