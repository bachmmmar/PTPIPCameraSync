
extern "C" {
  #include <unistd.h>
  #include <gphoto2/gphoto2-setting.h>
  #include <gphoto2/gphoto2-filesys.h>
}

#include <iostream>
#include "gphoto_helper.hpp"
#include "gphoto_exceptions.hpp"


#define THROW_ON_ERROR(function, message) {int __r=(function); if(__r != GP_OK) throw GPhotoException(message);}


void GPhotoHelper::enableDebugLog() {
    gp_log_add_func (GP_LOG_ALL, debug_func, (void *) NULL);
}


void GPhotoHelper::createContextWithCallbacksRegistered(GPContext *context) {
    context = gp_context_new();

    // Optional debugging and status output
    gp_context_set_error_func(context, ctx_error_func, NULL );
    gp_context_set_status_func(context, ctx_status_func, NULL );
}


void GPhotoHelper::createCamera(Camera **camera) {
    THROW_ON_ERROR(gp_camera_new(camera), "Failed to create a new empty camera!");
}


std::string GPhotoHelper::getSummary(Camera *camera, GPContext *context) {
    CameraText text;

    THROW_ON_ERROR(gp_camera_get_summary(camera, &text, context), "Failed to get camera summary!");
    return std::string(text.text);
}


StringList GPhotoHelper::getAllFilenames (Camera *camera, GPContext *context, std::string folder) {
    CameraList *list;
    const char *name;
    StringList filenames;

    THROW_ON_ERROR (gp_list_new (&list), "Could not create a new list.");
    int res = gp_camera_folder_list_files (camera, folder.c_str(), list, context);
    if (res != GP_OK) {
        gp_list_free (list);
        throw GPhotoException("Failed to list files in folder (" + folder + ") from camera!");
    }
    
    int filecount = gp_list_count (list);
    for (int i = 0; i < filecount; i++) {
        THROW_ON_ERROR(gp_list_get_name (list, i, &name), "Could not get filename!");
	filenames.push_back(std::string(name));
    }
    
    gp_list_free (list);
    return filenames;
}


StringList GPhotoHelper::getAllFolders (Camera *camera, GPContext *context, std::string folder) {
    CameraList *list;
    const char *name;
    StringList foldernames;
	
    THROW_ON_ERROR (gp_list_new (&list), "Could not create a new list.");
    int res = gp_camera_folder_list_folders(camera, folder.c_str(), list, context);
    if (res != GP_OK) {
        gp_list_free (list);
        throw GPhotoException("Failed to get folders in folder (" + folder + ") from camera!");
    }

    int count = gp_list_count (list); 
    for (int i = 0; i < count; i++) {
        THROW_ON_ERROR (gp_list_get_name(list, i, &name), "Could not get foldername!");
	foldernames.push_back(std::string(name));
    }
	
    gp_list_free (list);
    return foldernames;
}


void GPhotoHelper::downloadFile (Camera *camera, GPContext *context,
				 std::string folder, std::string filename,
				 std::string destination_file) {
    int fd, res;
    CameraFile *file;
    char tmpfile[] = "tmpfileXXXXXX";

    // create temporary file and directly open filedescriptor
    fd = mkstemp(tmpfile);
    if (fd == -1) {
        if (errno == EACCES) {
	    throw GPhotoException("Permission denied (" + std::string(tmpfile) + ")!");
	}
	throw GPhotoException("Could not create temp file (" + std::string(tmpfile) + ")!");
    } else {
        res = gp_file_new_from_fd (&file, fd);
	if (res != GP_OK) {
	    close (fd);
	    unlink (tmpfile);
	    throw GPhotoException("Permission denied (" + std::string(tmpfile) + ")!");
	}
    }

    CameraFileType type = GP_FILE_TYPE_NORMAL;
    res = gp_camera_file_get (camera, folder.c_str(), filename.c_str(), type, file, context);
    gp_file_unref (file);
    if (res < GP_OK) {
	unlink (tmpfile);
	throw GPhotoException("Could not get file from cam!");	    
    }

	
    res = std::rename(tmpfile, destination_file.c_str());
    if (res) {
        unlink (tmpfile);
	throw GPhotoException("Could not rename file!");
    }
}




void GPhotoHelper::setCameraModel (Camera *camera, GPContext *context, std::string model) {
    CameraAbilities a;
    CameraAbilitiesList * abilities_list = NULL;

    THROW_ON_ERROR(gp_abilities_list_new (&abilities_list), "Failed to create ability list!");
    if (gp_abilities_list_load (abilities_list, context) != GP_OK) {
        gp_abilities_list_free(abilities_list);
	throw GPhotoException("Failed to load abilities list!");
    }
	
    int lookup_path = gp_abilities_list_lookup_model(abilities_list, model.c_str());
    if (lookup_path == GP_ERROR_MODEL_NOT_FOUND) {
        gp_abilities_list_free(abilities_list);
	throw GPhotoException("The model you specified ('" + model + "') can not be " + \
			      "found. Please specify one of the models found by " + \
			      "'gphoto2 --list-cameras' and make sure the spelling " + \
			      "is correct.");
    }

    if (gp_abilities_list_get_abilities (abilities_list, lookup_path, &a) != GP_OK) {
        gp_abilities_list_free(abilities_list);
	throw GPhotoException("Failed to get selected camera abilities!");
    }

    gp_abilities_list_free(abilities_list);

    THROW_ON_ERROR(gp_camera_set_abilities (camera, a), "Failed to set camera abilities!");
}

void GPhotoHelper::setCameraPort (Camera *camera, std::string port) {
    GPPortInfoList *list = NULL;

    THROW_ON_ERROR (gp_port_info_list_new (&list), "Failed to create info list!");

    if (gp_port_info_list_load (list) != GP_OK) {
        gp_port_info_list_free (list);
        throw GPhotoException("failed to create info list!");
    }

    if (gp_port_info_list_count (list) < 0) {
        throw GPhotoException("failed to create info list!");
        gp_port_info_list_free (list);
    }

    // Search our port in the list.
    int lookup_path = gp_port_info_list_lookup_path (list, port.c_str());
    if (lookup_path == GP_ERROR_UNKNOWN_PORT) {
        gp_port_info_list_free (list);
        throw GPhotoException("The port you specified ('" + port + "') can not " + \
			      "be found. Please specify one of the ports found by " + \
			      "'gphoto2 --list-ports' and make sure the spelling " + \
			      "is correct (i.e. with prefix 'serial:' or 'usb:').");
    }

    GPPortInfo info;
    if (gp_port_info_list_get_info (list, lookup_path, &info) != GP_OK) {
        gp_port_info_list_free (list);
	throw GPhotoException("Failed to get port info!");
    }

    // Set the port of our camera.
    if (gp_camera_set_port_info(camera, info) != GP_OK) {
        gp_port_info_list_free (list);
	throw GPhotoException("Failed to set port in camera");
    }
    gp_port_info_list_free (list);
}

void GPhotoHelper::ctx_error_func (GPContext *context, const char *str, void *data) {
    std::cerr << "*** Contexterror ***" << std::endl;
    std::cerr << std::string(str) << std::endl;
}

void GPhotoHelper::ctx_status_func (GPContext *context, const char *str, void *data) {
    std::cout << "Status: " << std::string(str) << std::endl;
}

void GPhotoHelper::debug_func (GPLogLevel level, const char *domain, const char *str, void *data) {
    std::cout << "libgphoto debug - " << std::string(domain) << ", " << std::string(str) << std::endl;
}
  
