#!/usr/bin/python2

from datetime import datetime
import logging
import os
import sys
import time


import gphoto_if

def main():
    logging.basicConfig(
        format='%(levelname)s: %(name)s: %(message)s', level=logging.WARNING)

    gp = gphoto_if.GPhotoIf('Sony PTP', '192.168.1.120', False)

    print('Connected!')

    summary = gp.getSummary()
    print(summary)

    print('folders')
    base_dir = gp.getFoldernamesFromFolder('/')
    for fb in base_dir:
        print('get folders in /{}'.format(fb))
        dir1 = gp.getFoldernamesFromFolder('/'+fb)
        for f1 in dir1:
            print('get files in /{}/{}'.format(fb, f1))
            files = gp.getFilenamesFromFolder('/' + fb + '/' + f1)
            for f in files:
                print('- /{}/{}/{}'.format(fb,f1,f))

    print('all done!')
    time.sleep(1)


    return 0

if __name__ == "__main__":
    sys.exit(main())
