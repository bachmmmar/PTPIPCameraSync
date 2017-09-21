#pragma once

extern "C" {
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>
#include <gphoto2/gphoto2-setting.h>
#include <gphoto2/gphoto2-filesys.h>
}

class GPhotoHelper {

public:
  static void enableDebugLog();
  
  static void createContextWithCallbacksRegistered(GPContext *context);

  static void setCameraModel (Camera *camera, GPContext *context, const char *model);
  static void setCameraPort (Camera *camera, GPPortInfoList *list, const char *port);
  
  // calback functions
  static void ctx_error_func (GPContext *context, const char *str, void *data);
  static void ctx_status_func (GPContext *context, const char *str, void *data);
  static void debug_func (GPLogLevel level, const char *domain, const char *str, void *data);
};

