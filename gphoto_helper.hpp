#pragma once

extern "C" {
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>
}

#include "gphoto_typedef.hpp"


class GPhotoHelper {

public:
  static void enableDebugLog();
  
  static void createContextWithCallbacksRegistered(GPContext *context);

  static void setCameraModel (Camera *camera, GPContext *context, const char *model);
  static void setCameraPort (Camera *camera, GPPortInfoList *list, const char *port);

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

