import os
import sys
import ntpath

import toml

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
descriptor_file = "assets.toml"

def save_descriptor(descriptor):
    print(descriptor)
    f = open(os.path.join('.', build_dir, descriptor_file), "w")
    toml.dump(descriptor, f)
    f.close()

def get_platform():
    if sys.platform.startswith('win32'):
        return "windows"
    elif sys.platform.startswith('linux'):
        return "linux"
    elif sys.platform.startswith('darwin'):
        return "osx"

def path_leaf(path):
    head, tail = ntpath.split(path)
    return tail or ntpath.basename(head)
