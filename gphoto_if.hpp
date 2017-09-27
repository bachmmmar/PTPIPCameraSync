#pragma once

extern "C" {
  #include <gphoto2/gphoto2-camera.h>
  #include <gphoto2/gphoto2-context.h>
}

#include "gphoto_typedef.hpp"


class GPhotoIf {

public:
    /// \brief Creates an Interface to libgphoto and directly connects to the given camera.
    /// \param model Model of the camera you want to connect to (e.g. "Sony PTP", gphoto2 --list-cameras | grep -i "PTP")
    /// \param ip The IP address of the camera
    /// \param debug enable (true) or disable (false) the debug output of libgphoto
    GPhotoIf(std::string model, std::string ip, bool debug);
    ~GPhotoIf();

    /// \brief Get information about connected camera (gphoto2 --summary)
    std::string getSummary();

    /// \brief Get all filenames of a given folder on the camera
    /// \param folder Folder to look for files (e.g. "/" for root)
    StringList getFilenamesFromFolder(std::string folder);

    /// \brief Get all foldernames of a given folder on the camera
    /// \param folder Folder to look for folders (e.g. "/" for root)
    StringList getFoldernamesFromFolder(std::string folder);

    /// \brief Download a file from camera and write it to given destination
    /// \param camera_folder The folder on the camera to get the \ref camera_file from
    /// \param camera_file The filename on the camera to be transfered
    /// \param destination_file Filename including fileextension (e.g. ARW/JPG) and absolute or relative path
    void downloadFile(std::string camera_folder, std::string camera_file, std::string destination_file);

  
private:
    void openCamera();

    std::string ip_;
    std::string model_;
  
    Camera* camera_;
    GPContext *context_;

  bool camera_up_;
};
