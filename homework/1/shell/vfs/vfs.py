import os
import zipfile


class VirtualFileSystem:
    def __init__(self, zip_path, log_path):
        self.zip_path = zip_path
        self.log_path = log_path

        self.zip_file = zipfile.ZipFile(zip_path, 'a')
        self.current_dir = "~"

    def ls(self, path=None):
        content = set()

        if path is not None:
            if path[-1] == "/":
                path = path[:-1]
            full_path = self.current_dir + '/' + path if self.current_dir != "~" else "/" + path
        else:
            full_path = self.current_dir

        parts = full_path.split('/')[1:] if full_path != "~" else []
        all_items = self.zip_file.namelist()

        if parts:
            prefix = '/'.join(parts) + '/'
            for item in all_items:
                if item.startswith(prefix):
                    relative_path = item[len(prefix):].split('/')
                    if relative_path[0]:
                        content.add(relative_path[0])
        else:
            for item in all_items:
                item_parts = item.split('/')
                if item_parts[0]:
                    content.add(item_parts[0])

        return content

    def cd(self, path):
        if path == "..":
            if self.current_dir != "~":
                parts = self.current_dir.split('/')[1:-1]
                if not parts:
                    self.current_dir = "~"
                else:
                    self.current_dir = "~/" + "/".join(parts)
            return True

        if self.current_dir == "~":
            start = ""
        else:
            start = self.current_dir[2:] + "/"

        if any(item.startswith(start + path + '/') for item in self.zip_file.namelist()):
            self.current_dir += ("/" + path)
            return True

        return False

    def touch(self, filename):
        full_path = os.path.join(self.current_dir, filename)
        self.zip_file.writestr(full_path, '')

    def close(self):
        self.zip_file.close()
