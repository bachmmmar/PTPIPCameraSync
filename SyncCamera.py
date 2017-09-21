#!/usr/bin/python2

from datetime import datetime
import logging
import os
import sys


import gphoto_if

def main():
    logging.basicConfig(
        format='%(levelname)s: %(name)s: %(message)s', level=logging.WARNING)

    gp = gphoto_if.GPhotoIf('192.168.1.120')
    print(gp.greet())
    #cam = gphoto.GPhoto('ptpip:192.168.1.120')

    #cam.bar()


    return 0

if __name__ == "__main__":
    sys.exit(main())
