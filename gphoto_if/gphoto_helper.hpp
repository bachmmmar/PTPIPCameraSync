#pragma once

extern "C" {
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>
}

#include "gphoto_typedef.hpp"


class GPhotoHelper {

public:
  static void enableDebugLog();

  // configuration and initialization functions
  static void createContextWithCallbacksRegistered(GPContext *context);
  static void createCamera(Camera **camera);
  static void setCameraModel (Camera *camera, GPContext *context, std::string model);
  static void setCameraPort (Camera *camera, std::string port);

  // main interface functions
  static std::string getSummary (Camera *camera, GPContext *context);
  static StringList getAllFilenames (Camera *camera, GPContext *context, std::string folder);
  static StringList getAllFolders (Camera *camera, GPContext *context, std::string folder);
  static void downloadFile (Camera *camera, GPContext *context,
			    std::string folder, std::string filename,
			    std::string destination_file);
  
  // calback functions
  static void ctx_error_func (GPContext *context, const char *str, void *data);
  static void ctx_status_func (GPContext *context, const char *str, void *data);
  static void debug_func (GPLogLevel level, const char *domain, const char *str, void *data);
};

