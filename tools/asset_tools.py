import os
import shutil
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    SUCCESS = '\033[92m'
    WARNING = '\033[93m'
    ERROR = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

dirs = {
    "shaders": "shaders",
    "textures": "textures",
    "models": "models",
}

root_dir = "assets"
build_dir = "build_assets"

asset_data = {}

def build_assets():
    pass

def ensure_build_dir():
    build_assets = os.path.join('.', build_dir)
    origin_assets = os.path.join('.', root_dir)

    if not os.path.isdir(build_assets):
        print(' - Creating build asset dir at: %s' % build_assets)
        os.mkdir(build_assets)

    for d in dirs:
        complete_dir = os.path.join(build_assets, dirs[d])
        if not os.path.isdir(complete_dir):
            print(' - Creating %s dir' % d)
            os.mkdir(complete_dir)

def check_required_dir():
    root = os.path.join('.', root_dir)
    for d in dirs:
        complete_dir = os.path.join(root, dirs[d])
        if not os.path.isdir(complete_dir):
            print('ERROR: "%s" does not exist.' % complete_dir)
            return False
    return True

def get_platform():
    if sys.platform.startswith('win32'):
        return "windows"
    elif sys.platform.startswith('linux'):
        return "linux"
    elif sys.platform.startswith('darwin'):
        return "osx"

if __name__ == '__main__':
    if check_required_dir():
        ensure_build_dir()
        build_assets()
