import os
import sys
import ntpath

from shutil import copyfile

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
    "cubemaps": "cubemaps",
    "fonts": "fonts"
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
    '120',
    '130',
    '140',
    '150',
    '330',
    '400',
    '410',
    '420',
    '430',
    '440',
]

root_dir = "assets"
build_dir = "build_assets"
descriptor_file = "assets.toml"
settings_file = "settings.toml"

shader_types = ["vs", "fs", "sh"]
mesh_types = ["obj", "gltf", "glb"]
texture_types = ["bmp", "dds", "exr", "gif", "jpg", "hdr", "ktx", "png", "psd", "pvr", "tga"]
cubemap_types = ["toml"]
font_types = ["ttf", "otf"]

texture_descriptor_schema = {
    'mipmap': False,
    'filter': 'linear', # 'linear', 'point'
    'gamma': 'gamma', # 'gamma', 'linear'
    'normalmap': False
}

mesh_descriptor_schema = {
    'normals': 'auto' # 'auto', 'generate', 'generate_soft'
}

shader_descriptor_schema = {
    'uniforms': []
}

def get_shadermodel_by_renderer(renderer):
    if renderer == "directx11": return "s_4_0"
    if renderer == "directx12": return "s_4_0"
    if renderer == "vulkan": return "spirv"
    if renderer == "metal": return "metal"
    return "410"

# this returns if the renderer changed and what the renderer is
def import_settings():
    source = os.path.join('.', root_dir, settings_file)
    dest = os.path.join('.', build_dir, settings_file)

    f = open(source, 'r')
    source_settings = toml.load(f)
    f.close()

    renderer_changed = False

    # check if renderer changed, useful for watchdog to recompile shaders
    if os.path.exists(dest):
        f = open(dest, 'r')
        dest_settings = toml.load(f)
        f.close()
        if source_settings['renderer'] != dest_settings['renderer']: renderer_changed = True

    copy = copyfile(source, dest)

    if copy:
        print(f" {bcolors.SUCCESS}- Settings imported: {dest}{bcolors.ENDC}")
    else:
        print(f" {bcolors.ERROR}- Problem importing settings: {dest}{bcolors.ENDC}")

    return renderer_changed, source_settings['renderer']


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
    if descriptor != None and os.path.isfile(descriptor): f = os.path.getmtime(output) < os.path.getmtime(descriptor)

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
