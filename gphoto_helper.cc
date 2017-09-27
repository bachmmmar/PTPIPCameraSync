
extern "C" {
  #include <unistd.h>
  #include <string.h>
  //  #include <stdio.h>
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
    // Create a context if necessary
    context = gp_context_new();

    // Optional debugging and status output
    gp_context_set_error_func(context, ctx_error_func, NULL );
    gp_context_set_status_func(context, ctx_status_func, NULL );
}


std::string GPhotoHelper::getSummary(Camera *camera, GPContext *context) {
   	CameraText text;

	if (gp_camera_get_summary(camera, &text, context) != GP_OK) {
	   throw GPhotoException("Failed to get camera summary!");
	}

	return std::string(text.text);
}


StringList GPhotoHelper::getAllFilenames (Camera *camera, GPContext *context, std::string folder) {
	CameraList *list;
	const char *name;
	StringList filenames;

	THROW_ON_ERROR (gp_list_new (&list), "Could not create a new list.");
	gp_camera_folder_list_files (camera, folder.c_str(), list, context);
	int filecount = gp_list_count (list);

	for (int i = 0; i < filecount; i++) {
	  gp_list_get_name (list, i, &name);
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
	THROW_ON_ERROR(gp_camera_folder_list_folders(camera, folder.c_str(), list, context), "Could not get folders");
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
	//	std::string tmpfile = "tmpfileXXXXXX";

	char tmpfile[] = "tmpfileXXXXXX";
	fd = mkstemp(tmpfile);
	if (fd == -1) {
	    if (errno == EACCES) {
	      throw GPhotoException("Permission denied (" + std::string(tmpfile) + ")!");
	    }
	    throw GPhotoException("Could not create temporary file (" + std::string(tmpfile) + ")!");
	} else {
	    res = gp_file_new_from_fd (&file, fd);
	    if (res < GP_OK) {
	        close (fd);
		unlink (tmpfile);
		throw GPhotoException("Permission denied (" + std::string(tmpfile) + ")!");
	    }
	}

	CameraFileType type = GP_FILE_TYPE_NORMAL;
        res = gp_camera_file_get (camera, folder.c_str(), filename.c_str(), type, file, context);
	if (res < GP_OK) {
	    gp_file_unref (file);
	    unlink (tmpfile);
	    throw GPhotoException("Could not get file from cam!");	    
	}

	gp_file_unref (file);
	
	res = std::rename(tmpfile, destination_file.c_str());
	if (res) {
	  unlink (tmpfile);
	  throw GPhotoException("Could not rename file!");
	}
}




void GPhotoHelper::setCameraModel (Camera *camera, GPContext *context, const char *model) {
    CameraAbilities a;
    CameraAbilitiesList * abilities_list = NULL;

    gp_abilities_list_new (&abilities_list);
    if (gp_abilities_list_load (abilities_list, context) != GP_OK) {
      std::cerr << "Failed to load abilities list!" << std::endl;
    }
	
    int lookup_path = gp_abilities_list_lookup_model(abilities_list, model);
    if (lookup_path == GP_ERROR_MODEL_NOT_FOUND) {
      std::cerr << "The model you specified ('" << model <<
        "') can not be found. Please specify one of the models found by " <<
        "'gphoto2 --list-cameras' and make sure the spelling is correct." << std::endl;
    }

    if (gp_abilities_list_get_abilities (abilities_list, lookup_path, &a) != GP_OK) {
      std::cerr << "Failed to get selected camera abilities!" << std::endl;
    }

    if (gp_camera_set_abilities (camera, a) != GP_OK) {
      std::cerr << "Failed to set camera abilities" << std::endl;
    }
    //	gp_setting_set ("gphoto2", "model", a.model);

}

void GPhotoHelper::setCameraPort (Camera *camera, GPPortInfoList *list, const char *port) {
	if (list)  {
	  std::cerr << "List has allready been used" << std::endl;
	}

	if (gp_port_info_list_new (&list) != GP_OK) {
	  std::cerr << "failed to create info list!" << std::endl;
	}

	if (gp_port_info_list_load (list) != GP_OK) {
	  std::cerr << "failed to create info list!" << std::endl;	  
	  gp_port_info_list_free (list);
	}

	if (gp_port_info_list_count (list) < 0) {
	  std::cerr << "failed to create info list!" << std::endl;
	  gp_port_info_list_free (list);
	}

	/* Search our port in the list. */
	/* NOTE: This call can modify "il" for regexp matches! */
	int lookup_path = gp_port_info_list_lookup_path (list, port);
	if (lookup_path == GP_ERROR_UNKNOWN_PORT) {
	  std::cerr << "The port you specified ('" << port <<
	    "') can not be found. Please specify one of the ports found by " <<
	    "'gphoto2 --list-ports' and make sure the spelling is correct " <<
	    "(i.e. with prefix 'serial:' or 'usb:')." << std::endl;
	}

	/* Get info about our port. */
	GPPortInfo info;
	if (gp_port_info_list_get_info (list, lookup_path, &info) != GP_OK) {
	  std::cerr << "failed to get info" << std::endl;
	}

	/* Set the port of our camera. */
	if (gp_camera_set_port_info(camera, info) != GP_OK) {
	  std::cerr << "failed to set port in camera" << std::endl;
	}
}

void GPhotoHelper::ctx_error_func (GPContext *context, const char *str, void *data) {
    std::cerr << "*** Contexterror ***" << std::endl;
    std::cerr << std::string(str) << std::endl;
}

void GPhotoHelper::ctx_status_func (GPContext *context, const char *str, void *data) {
    std::cout << "Status: " << std::string(str) << std::endl;
}

void GPhotoHelper::debug_func (GPLogLevel level, const char *domain, const char *str, void *data) {

  std::cout << "Debug - " << std::string(domain) << ", " << std::string(str) << std::endl;
  /*
  struct timeval tv;
	long sec, usec;
	FILE *logfile = (data != NULL)?(FILE *)data:stderr;

	gettimeofday (&tv,NULL);
	sec = tv.tv_sec  - glob_tv_zero.tv_sec;
	usec = tv.tv_usec - glob_tv_zero.tv_usec;
	if (usec < 0) {sec--; usec += 1000000L;}
	fprintf (logfile, "%li.%06li %-28s(%i): %s\n", sec, usec, domain, level, str);
  */
}
  
