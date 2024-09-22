import os
import zipfile
import re
import xml.etree.ElementTree as ET


class VirtualFileSystem:
    def __init__(self, zip_path, log_path):
        self.zip_path = zip_path
        self.log_path = log_path

        self.zip_file = zipfile.ZipFile(zip_path, 'a')
        self.current_dir = "~"
        self.permissions_file = "permissions.txt"
        self.permissions = self.load_permissions()
        self.log_root = ET.Element("session")

    def load_permissions(self):
        permissions = {}
        if os.path.exists(self.permissions_file):
            with open(self.permissions_file, 'r') as f:
                for line in f:
                    file, perm = line.strip().split()
                    permissions[file] = perm
        return permissions

    def save_permissions(self):
        with open(self.permissions_file, 'w') as f:
            for file, perm in self.permissions.items():
                f.write(f"{file} {perm}\n")

    def log_action(self, command, result):
        action = ET.SubElement(self.log_root, "action")
        ET.SubElement(action, "command").text = command
        ET.SubElement(action, "result").text = result

    def save_log(self):
        tree = ET.ElementTree(self.log_root)
        tree.write(self.log_path)

    def chmod(self, filename, permissions):
        if not re.match(r'^[rwx-]{9}$', permissions):
            result = "Invalid permissions format. Use rwxr-xr-x format."
            self.log_action(f"chmod {filename} {permissions}", result)
            return result

        # Determine the full path for the file
        if self.current_dir == "~":
            full_path = filename
        else:
            full_path = os.path.join(self.current_dir[2:], filename)

        # Check if the file exists in the current directory
        if full_path not in self.zip_file.namelist():
            result = f"File {filename} not found in the current directory."
            self.log_action(f"chmod {filename} {permissions}", result)
            return result

        # Update permissions
        self.permissions[full_path] = permissions
        self.save_permissions()
        result = f"Permissions for {filename} set to {permissions}"
        self.log_action(f"chmod {filename} {permissions}", result)
        return result

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

        result = " ".join(content)
        self.log_action(f"ls {path if path else ''}", result)
        return content

    def cd(self, path):
        if path == "..":
            if self.current_dir != "~":
                parts = self.current_dir.split('/')[1:-1]
                if not parts:
                    self.current_dir = "~"
                else:
                    self.current_dir = "~/" + "/".join(parts)
            self.log_action(f"cd {path}", "Success")
            return True

        if self.current_dir == "~":
            start = ""
        else:
            start = self.current_dir[2:] + "/"

        if any(item.startswith(start + path + '/') for item in self.zip_file.namelist()):
            self.current_dir += ("/" + path)
            self.log_action(f"cd {path}", "Success")
            return True

        self.log_action(f"cd {path}", "Directory not found")
        return False

    def touch(self, filename):
        if self.current_dir == "~":
            full_path = filename
        else:
            full_path = os.path.join(self.current_dir[2:], filename)

        if full_path in self.zip_file.namelist():
            self.log_action(f"touch {filename}", "File already exists")
            return

        self.zip_file.writestr(full_path, '')
        self.permissions[full_path] = "rw-r--r--"
        self.save_permissions()
        self.log_action(f"touch {filename}", f"File {filename} created.")

    def close(self):
        self.zip_file.close()
        self.save_log()
