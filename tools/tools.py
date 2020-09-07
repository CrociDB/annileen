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
    "cubemaps": "cubemaps"
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
    's_3_0',
    's_4_0',
    's_4_0_level',
    's_5_0',
    'metal',
    'pssl',
    'spirv',
    '120'
]

root_dir = "assets"
build_dir = "build_assets"
descriptor_file = "assets.toml"

shader_types = ["vs", "fs"]
mesh_types = ["obj", "gltf", "glb"]
texture_types = ["bmp", "dds", "exr", "gif", "jpg", "hdr", "ktx", "png", "psd", "pvr", "tga"]
cubemap_types = ["toml"]

texture_descriptor_schema = {
    'mipmap': False,
    'filter': 'linear'
}

mesh_descriptor_schema = {
    'normals': 'auto' # 'auto', 'generate', 'generate_soft'
}

def save_descriptor(descriptor):
    descriptor_path = os.path.join('.', build_dir, descriptor_file)
    f = open(descriptor_path, "w")
    toml.dump(descriptor, f)
    f.close()
    return descriptor_path
    
def save_asset_descriptor(file_path, descriptor):
    f = open(file_path, "w")
    toml.dump(descriptor, f)
    f.close()

def save_built_asset_descriptor(asset_path, descriptor):
    (head, _) = ntpath.split(asset_path)
    descriptor_file = os.path.join(head, path_leaf(asset_path.split('.')[0]) + '.toml')
    save_asset_descriptor(descriptor_file, descriptor)

def load_asset_descriptor(asset_path, schema):
    (head, _) = ntpath.split(asset_path)
    descriptor_file = os.path.join(head, path_leaf(asset_path.split('.')[0]) + '.toml')

    if not os.path.isfile(descriptor_file):
        # save_asset_descriptor(descriptor_file, schema)
        return descriptor_file, schema

    return descriptor_file, _load_file_descriptor(descriptor_file)
    
def _load_file_descriptor(file_path):
    f = open(file_path, "r")
    descriptor = toml.load(f)
    f.close()
    return descriptor

def check_should_build(output, input, descriptor=None):
    if not os.path.isfile(output): return True
    f = os.path.getmtime(output) < os.path.getmtime(input)
    d = True
    if descriptor != None: f = os.path.getmtime(output) < os.path.getmtime(descriptor)

    return f and d

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
