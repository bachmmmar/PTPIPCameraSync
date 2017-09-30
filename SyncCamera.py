#!/usr/bin/python2

from datetime import datetime
import logging
import argparse
import os
import sys
import time

from modules import *

import gphoto_if

class SyncCamera():
    def __init__(self):
        logging.basicConfig(
            format='%(levelname)s: %(name)s: %(message)s', level=logging.DEBUG)
        self._log = logging.getLogger('SyncCamera')

        self._arg = getParsedArguments()

        #check arguments
        create_datafile(self._arg.datafile, self._arg.create_datafile)
        is_valid_directory(self._arg.incomming_folder, 'Inncomming Directory is not existing!')
        is_valid_file(self._arg.datafile, 'Datafile is not existing!')

        self._sig = CheckSignals()
        self._cam_finder = CameraFinder(self._arg.ip)

    def run(self):
        while not self._sig.terminationSignalReceived():
            if self._cam_finder.isReachable():
                self._log.debug('Camera found! Start synchronization...')
                if self.syncCamera() is True:
                    print 'All files successfully synchronized!'
                    time.sleep(20)
                else:
                    print 'An error ocured. Synchronization abborded!'
            time.sleep(3)


    def syncCamera(self):
        try:
            gp = gphoto_if.GPhotoIf(self._arg.model, self._arg.ip, False)
            db = DataBase(self._arg.datafile)
            sync = CameraSynchronizer(gp,db)
            while not self._sig.terminationSignalReceived():
                if sync.processFile(self._arg.incomming_folder) is False:
                    self._log.debug('All files synchronized!')
                    return True

        except UserWarning as e:
            self._log.error('Error while syncing data! {}'.format(e.message))
            return False


def getParsedArguments():
    parser = argparse.ArgumentParser(
        description='Waits till defined camera is available and copy all new images to defined foler.', \
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument('-i', '--incomming_folder', default='incomming_folder', \
                        help='directory where received images are placed')

    parser.add_argument('--datafile', default='copied_images.txt', \
                        help='file containing all allready copied images')
    parser.add_argument('-c', '--create_datafile', default=False, action='store_true',
                             help='Creates a new empty datafile')

    parser_cam = parser.add_argument_group('CameraConfiguration', 'Arguments affecting camera.')
    parser_cam.add_argument('--ip', default='192.168.1.120', \
                            help='Cameras IP address')
    parser_cam.add_argument('--model', default='Sony PTP', \
                            help='Camera model')

    return parser.parse_args()


def is_valid_directory(dir, text, create=False):
    if create and not os.path.exists(dir):
        os.mkdir(dir)
    if not os.path.isdir(dir):
        print('Error: {} ({})'.format(text, dir))
        sys.exit(1)


def is_valid_file(file, text):
    if not os.path.isfile(file):
        print('Error: {} ({})'.format(text, file))
        sys.exit(1)

def create_datafile(file, create):
    if create:
        with open(file, 'w') as f:
            f.write('SyncCamera/Datafile\n')

if __name__ == "__main__":
    s = SyncCamera()
    sys.exit(s.run())
