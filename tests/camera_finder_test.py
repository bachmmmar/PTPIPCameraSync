import unittest

from modules import CameraFinder

class CameraFinderTest(unittest.TestCase):
    def test_create_class_with_ip_works(self):
        cut = CameraFinder('192.168.1.1')
        self.assertIsInstance(cut, CameraFinder)

    def test_create_class_with_invalid_ip_throws_exception(self):
        self.assertRaises(Exception, CameraFinder,'78df.erlg.r')

    def test_create_and_ping_unreachable_host(self):
        cut = CameraFinder('212.212.255.0') # Normally not reachable
        self.assertFalse(cut.isReachable())

    def test_create_and_ping_a_reachable_host(self):
        cut = CameraFinder('0.0.0.0') # Allways gives an answer
        self.assertTrue(cut.isReachable())