import tkinter as tk
from tkinter import scrolledtext
from .vfs.vfs import VirtualFileSystem


class ShellEmulator:
    def __init__(self, zip_path, log_path):
        self.vfs = VirtualFileSystem(zip_path, log_path)

    def execute_command(self, input):
        parts = input.split()
        command = parts[0]
        args = parts[1:] if len(parts) > 1 else []

        if command == "exit":
            return "exit"
        elif command == "cd":
            if len(args) == 1:
                if not self.vfs.cd(args[0]):
                    return "Directory not found"
            else:
                return "Usage: cd <directory_path>"
        elif command == "ls":
            if len(args) < 2:
                if len(args) == 1:
                    content = self.vfs.ls(args[0])
                else:
                    content = self.vfs.ls()
                return " ".join(content)
            else:
                return "Usage: ls <path> or nothing"
        elif command == "touch":
            if len(args) == 1:
                self.vfs.touch(args[0])
                return f"File {args[0]} created."
            else:
                return "Usage: touch <filename>"
        elif command == "chmod":
            if len(args) == 2:
                return self.vfs.chmod(args[0], args[1])
            else:
                return "Usage: chmod <filename> <permissions>"
        else:
            return self.check_file(command)

    def check_file(self, filename):
        return self.vfs.check_file_in_permissions(filename)


class ShellGUI:
    def __init__(self, zip_path, log_path):
        self.root = tk.Tk()
        self.root.title("Shell Emulator")
        self.emulator = ShellEmulator(zip_path, log_path)

        # Set the background color to black
        self.root.configure(bg='black')

        # Create a text area for output
        self.output_area = scrolledtext.ScrolledText(self.root, wrap=tk.WORD, width=80, height=20, bg='black', fg='white')
        self.output_area.grid(column=0, row=0, padx=10, pady=10, columnspan=2)
        self.output_area.config(state=tk.DISABLED)  # Make it read-only

        # Create an entry widget for command input
        self.command_entry = tk.Entry(self.root, width=70, bg='black', fg='white', insertbackground='white')
        self.command_entry.grid(column=0, row=1, padx=10, pady=10)
        self.command_entry.bind('<Return>', self.send_command)  # Bind Enter key

        # Create a button to send the command
        self.send_button = tk.Button(self.root, text="Send", command=self.send_command, bg='black', fg='white')
        self.send_button.grid(column=1, row=1, padx=10, pady=10)

        # Bind the close event to ensure the log is saved
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)

    def start(self):
        self.root.mainloop()

    def send_command(self, event=None):
        command = self.command_entry.get()
        if command:
            result = self.emulator.execute_command(command)
            self.output_area.config(state=tk.NORMAL)
            self.output_area.insert(tk.END, f"{self.emulator.vfs.current_dir}> {command}\n")
            if result == "exit":
                self.root.destroy()
                return "break"
            elif result:
                self.output_area.insert(tk.END, f"{result}\n")
            self.output_area.config(state=tk.DISABLED)
            self.command_entry.delete(0, tk.END)

    def on_close(self):
        self.emulator.vfs.close()
        self.root.destroy()
