import unittest

testmodules = [
    'gphoto_if_test.GPhotoIfTest',
    'data_base_test.DataBaseTest',
    'camera_finder_test.CameraFinderTest'
    ]

if __name__ == '__main__':
    suite = unittest.TestSuite()

    for t in testmodules:
        try:
            # If the module defines a suite() function, call it to get the suite.
            mod = __import__(t, globals(), locals(), ['suite'])
            suitefn = getattr(mod, 'suite')
            suite.addTest(suitefn())
        except (ImportError, AttributeError):
            # else, just load all the test cases from the module.
            suite.addTest(unittest.defaultTestLoader.loadTestsFromName(t))

    unittest.TextTestRunner().run(suite)

