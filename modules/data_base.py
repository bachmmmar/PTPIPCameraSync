import os
class DataBase():

    def _doesFileExists(self, file):
        if not os.path.isfile(file):
            raise Exception('Database file {} does not exists!'.format(file))

    def _readFile(self,filename):
        with open(filename, 'r') as f:
            for line in f:
                self._content.append(str(line).strip())

    def __init__(self, filename):
        self._content = list()
        self._db_filename = filename
        self._doesFileExists(filename)
        self._readFile(filename)

    def addItem(self, data_str):
        ds = str(data_str).strip()
        with open(self._db_filename, 'a') as f:
            f.write('{}\n'.format(ds))
        self._content.append(ds)

    def containsItem(self, data_str):
        ds = str(data_str).strip()
        if ds in self._content:
            return True
        else:
            return False
