import os

class CameraFinder():
    PING_OPTIONS = '-q -c 1 -W 2'

    def __init__(self, ip):
        if not CameraFinder.isIpValid(ip):
            raise Exception('ip {} is invalid'.format(ip))
        self._ip = ip
        self._ping_options = CameraFinder.PING_OPTIONS

    def isReachable(self):
        ret = os.system('ping {} {} > /dev/null 2>&1'.format(self._ping_options, self._ip))
        if ret != 0:
            return False
        else:
            return True

    @staticmethod
    def isIpValid(ip):
        a = ip.split('.')
        if len(a) != 4:
            return False
        for x in a:
            if not x.isdigit():
                return False
            i = int(x)
            if i < 0 or i > 255:
                return False
        return True
