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

available_platforms = [
    'auto',
    'android',
    'asm.js',
    'ios',
    'linux',
    'orbis',
    'osx',
    'windows',
]

available_shader_models = [
    'auto',
    's_3',
    's_4',
    's_4_0_level',
    's_5',
    'metal',
    'pssl',
    'spirv',
]

root_dir = "assets"
build_dir = "build_assets"
descriptor_file = "assets.toml"

shader_types = ["vs", "fs"]
mesh_types = ["obj", "gltf", "glb"]
texture_types = ["bmp", "dds", "exr", "gif", "jpg", "hdr", "ktx", "png", "psd", "pvr", "tga"]

def save_descriptor(descriptor):
    descriptor_path = os.path.join('.', build_dir, descriptor_file)
    f = open(descriptor_path, "w")
    toml.dump(descriptor, f)
    f.close()
    return descriptor_path
    

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
