import argparse
from shell.emulator import ShellGUI

def parse_arguments():
    parser = argparse.ArgumentParser(description="Shell Emulator with Virtual File System")
    parser.add_argument("zip_path", help="Path to the zip file containing the virtual file system")
    parser.add_argument("log_path", help="Path to the log file where session actions will be recorded")
    return parser.parse_args()

def main():
    args = parse_arguments()
    app = ShellGUI(args.zip_path, args.log_path)
    app.start()

if __name__ == "__main__":
    main()
