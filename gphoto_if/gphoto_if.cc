#include <iostream>

#include "gphoto_if.hpp"
#include "gphoto_python_exports.hpp"
#include "gphoto_helper.hpp"
  

GPhotoIf::GPhotoIf(std::string model, std::string ip, bool debug) :
    ip_(ip),
    model_(model),
    camera_(NULL),
    context_(NULL) ,
    camera_up_(false) {
    if (debug) {
       GPhotoHelper::enableDebugLog();
    }
    
    openCamera();
    camera_up_ = true;
}


GPhotoIf::~GPhotoIf() {
    if (camera_up_) {
        gp_camera_exit(camera_, context_);
    }

    if (camera_) {
        gp_camera_unref (camera_);
    }

    if (context_) {
      gp_context_unref (context_);
    }
}


std::string GPhotoIf::getSummary() {
    return GPhotoHelper::getSummary(camera_, context_);
}


StringList GPhotoIf::getFilenamesFromFolder(std::string folder) {
    return GPhotoHelper::getAllFilenames(camera_, context_, folder);
}


StringList GPhotoIf::getFoldernamesFromFolder(std::string folder) {    
    return GPhotoHelper::getAllFolders(camera_, context_, folder);
}


void GPhotoIf::downloadFile(std::string camera_folder, std::string camera_file, std::string destination_file) {
    GPhotoHelper::downloadFile(camera_, context_,
			       camera_folder, camera_file,
			       destination_file);
}


void GPhotoIf::openCamera() {
    GPhotoHelper::createContextWithCallbacksRegistered(context_);

    GPhotoHelper::createCamera(&camera_);

    std::string port = "ptpip:" + ip_;
    GPhotoHelper::setCameraPort(camera_, port);

    GPhotoHelper::setCameraModel(camera_, context_, model_.c_str());

    if (gp_camera_init(camera_, context_) != GP_OK) {
      throw GPhotoException("Camera init failed!");
    }
}


