import unittest
import os
import shutil
import zipfile
from shell.vfs.vfs import VirtualFileSystem

class TestVirtualFileSystem(unittest.TestCase):
    def setUp(self):
        self.test_dir = "test"
        os.makedirs(self.test_dir, exist_ok=True)

        self.zip_path = os.path.join(self.test_dir, "test.zip")
        self.log_path = os.path.join(self.test_dir, "test_log.xml")

        with zipfile.ZipFile(self.zip_path, 'w') as zf:
            zf.writestr("test.txt", "content")

        self.vfs = VirtualFileSystem(self.zip_path, self.log_path)

    def tearDown(self):
        self.vfs.close()
        if os.path.exists(self.vfs.permissions_file):
            os.remove(self.vfs.permissions_file)
        if os.path.exists(self.test_dir):
            shutil.rmtree(self.test_dir)

    def test_ls_touch(self):
        self.assertIn("test.txt", self.vfs.ls())

        self.vfs.touch("test1.txt")
        self.vfs.touch("test2.txt")
        self.assertIn("test1.txt", self.vfs.ls())
        self.assertIn("test2.txt", self.vfs.ls())

        self.vfs.touch("subdir/")
        self.vfs.cd("subdir")
        self.vfs.touch("subfile.txt")
        self.assertIn("subfile.txt", self.vfs.ls())

        self.vfs.cd("..")
        self.assertIn("subdir", self.vfs.ls())

    def test_cd(self):
        self.assertTrue(self.vfs.cd(".."))
        self.assertFalse(self.vfs.cd("nonexistent"))

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
