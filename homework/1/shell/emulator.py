from .vfs.vfs import VirtualFileSystem


class ShellEmulator:
    def __init__(self, zip_path, log_path):
        self.vfs = VirtualFileSystem(zip_path, log_path)

    def start(self):
        while True:
            self.show_path_start_line()
            input_line = input().strip()
            if input_line:
                should_exit = self.execute_command(input_line)
                if should_exit:
                    break

    def execute_command(self, input):
        parts = input.split()
        command = parts[0]
        args = parts[1:] if len(parts) > 1 else []

        if command == "exit":
            return True
        elif command == "cd":
            if len(args) == 1:
                if not self.vfs.cd(args[0]):
                    print("Directory not found")
            else:
                print("Usage: cd <directory_path>")
        elif command == "ls":
            if len(args) < 2:
                if len(args) == 1:
                    content = self.vfs.ls(args[0])
                else:
                    content = self.vfs.ls()
                print(" ".join(content))
            else:
                print("Usage: ls <path> or nothing")

        return False

    def show_path_start_line(self):
        print(self.vfs.current_dir + "> ", end='')