import unittest

from modules import DataBase

class DataBaseTest(unittest.TestCase):

    def createDataBase(self):
        filename = '/tmp/my_test.csv'
        with open(filename,'w') as f:
            f.write('hallo/test\n')
        return filename

    def test_create_class_with_valid_file_succeeds(self):
        filename = self.createDataBase()
        cut = DataBase(filename)
        self.assertIsInstance(cut, DataBase)

    def test_create_class_with_invalid_file_fails(self):
        self.assertRaises(Exception, DataBase, '/tmp/i_do_not_exist')

    def test_check_if_value_is_in_database_succeeds(self):
        filename = self.createDataBase()
        cut = DataBase(filename)
        self.assertTrue(cut.containsItem("hallo/test"))

    def test_check_if_value_is_in_database_fails(self):
        filename = self.createDataBase()
        cut = DataBase(filename)
        self.assertFalse(cut.containsItem("halo/tes"))

    def test_add_value_and_could_be_found(self):
        filename = self.createDataBase()
        cut = DataBase(filename)
        cut.addItem("test/if/im/in")
        cut.addItem("test/me/too")

        self.assertTrue(cut.containsItem("hallo/test"))
        self.assertTrue(cut.containsItem("test/if/im/in"))
        self.assertTrue(cut.containsItem("test/me/too"))

        cut2 = DataBase(filename)
        self.assertTrue(cut2.containsItem("hallo/test"))
        self.assertTrue(cut2.containsItem("test/if/im/in"))
        self.assertTrue(cut2.containsItem("test/me/too"))