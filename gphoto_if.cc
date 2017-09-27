#include <iostream>


#include "gphoto_if.hpp"
#include "gphoto_python_exports.hpp"
#include "gphoto_helper.hpp"
  

GPhotoIf::GPhotoIf(std::string model, std::string ip, bool debug) :
    ip_(ip),
    model_(model),
    camera_(NULL),
    context_(NULL),
    port_info_list_(NULL) {

  if (debug) {
    GPhotoHelper::enableDebugLog();
  }
    openCamera();
  }


  GPhotoIf::~GPhotoIf() {
    gp_camera_exit(camera_, context_);
  }


  std::string GPhotoIf::getSummary() {
    return GPhotoHelper::getSummary(camera_, context_);
  }


StringList GPhotoIf::getFilenamesFromFolder(std::string folder)  {
  return GPhotoHelper::getAllFilenames(camera_, context_, folder);
  }


StringList GPhotoIf::getFoldernamesFromFolder(std::string folder) {    
    return GPhotoHelper::getAllFolders(camera_, context_, folder);
  }


void GPhotoIf::downloadFile(std::string camera_folder, std::string camera_file, std::string destination_file) {
  GPhotoHelper::downloadFile (camera_, context_,
			      camera_folder, camera_file,
			      destination_file);
}

  void GPhotoIf::openCamera() {
    std::cout << "Open Camera on ptp:<" << ip_ << ">" << std::endl;

    GPhotoHelper::createContextWithCallbacksRegistered(context_);


    // Create new camera
    if( gp_camera_new( &camera_ ) != GP_OK ) {
      throw GPhotoException("Failed to create a new empty camera!");
    }

    std::string port = "ptpip:"+ip_;
    GPhotoHelper::setCameraPort(camera_, port_info_list_, port.c_str());

    GPhotoHelper::setCameraModel(camera_, context_, model_.c_str());
				 
    std::cout << "Camera init.  Takes about 10 seconds." << std::endl;
    
    if (gp_camera_init(camera_, context_) != GP_OK) {
      std::cout << "camera init failed " << std::endl;
      return;
    }

      std::cout << "camera created!" << std::endl;
  }


