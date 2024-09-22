import unittest
import os
import zipfile
from shell.vfs.vfs import VirtualFileSystem

class TestVirtualFileSystem(unittest.TestCase):
    def setUp(self):
        self.zip_path = "test.zip"
        self.log_path = "test_log.xml"
        with zipfile.ZipFile(self.zip_path, 'w') as zf:
            zf.writestr("test.txt", "content")
        self.vfs = VirtualFileSystem(self.zip_path, self.log_path)

    def tearDown(self):
        self.vfs.close()
        os.remove(self.zip_path)
        if os.path.exists(self.log_path):
            os.remove(self.log_path)
        if os.path.exists(self.vfs.permissions_file):
            os.remove(self.vfs.permissions_file)

    def test_ls(self):
        self.assertIn("test.txt", self.vfs.ls())

    def test_cd(self):
        self.assertTrue(self.vfs.cd(".."))
        self.assertFalse(self.vfs.cd("nonexistent"))

    def test_touch(self):
        self.vfs.touch("newfile.txt")
        self.assertIn("newfile.txt", self.vfs.ls())

    def test_chmod(self):
        self.vfs.touch("chmodfile.txt")
        result = self.vfs.chmod("chmodfile.txt", "rwxr-xr--")
        self.assertEqual(result, "Permissions for chmodfile.txt set to rwxr-xr--")
        result_invalid = self.vfs.chmod("chmodfile.txt", "invalid")
        self.assertEqual(result_invalid, "Invalid permissions format. Use rwxr-xr-x format.")

    def test_exit(self):
        self.vfs.close()
        self.assertTrue(os.path.exists(self.log_path))

if __name__ == "__main__":
    unittest.main()
