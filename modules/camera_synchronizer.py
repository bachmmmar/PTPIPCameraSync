import os
import logging
import gphoto_if
from modules import DataBase

class CameraSynchronizer():

    def __init__(self, gp_if, db):
        self._log = logging.getLogger('CameraSynchronizer')
        assert isinstance(gp_if, gphoto_if.GPhotoIf)
        assert isinstance(db, DataBase)
        self._gp_if = gp_if
        self._db = db
        self._file_list = self.listAllFilesOnCamera()

    def processFile(self, destination_path):
        if len(self._file_list) > 0:
            f = self._file_list.pop(0)
            if self._db.containsItem(f.getDBFilePath()):
                self._log.debug('The file {} is allready in the DB!'.format(f.getDBFilePath()))
                return True
            else:
                destination = os.path.join(destination_path, f.getFile())
                self._log.debug('The file {} new! lets get it...'.format(f.getDBFilePath()))
                self._gp_if.downloadFile(f.getFolder(), f.getFile(), destination)
                self._log.debug('Got file {}!'.format(f.getDBFilePath()))
                self._db.addItem(f.getDBFilePath())
                return True
        return False

    def listAllFilesOnCamera(self):
        file_list=list()
        base_dir = self._gp_if.getFoldernamesFromFolder('/')
        for fb in base_dir:
            self._log.debug('get folders in /{}'.format(fb))
            dir1 = self._gp_if.getFoldernamesFromFolder('/' + fb)
            for f1 in dir1:
                self._log.debug('get files in /{}/{}'.format(fb, f1))
                files = self._gp_if.getFilenamesFromFolder('/' + fb + '/' + f1)
                for f in files:
                    self._log.debug('file found: /{}/{}/{}'.format(fb, f1, f))
                    file_list.append(DirectoryStructure(fb,f1,f))
        return file_list

class DirectoryStructure():
    def __init__(self, base_folder, second_folder, file):
        self._base_folder = base_folder
        self._second_folder = second_folder
        self._file = file

    def getDBFilePath(self):
        return '{}/{}'.format(self._second_folder,self._file)

    def getFile(self):
        return self._file

    def getFolder(self):
        return '/{}/{}/'.format(self._base_folder, self._second_folder)