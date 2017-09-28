import unittest
import gphoto_if

class GPhotoIfTest(unittest.TestCase):

    def test_creation_with_invalid_param_throws_exception(self):
        self.assertRaises(UserWarning, gphoto_if.GPhotoIf,'no valid model', 'no ip addr', False)
