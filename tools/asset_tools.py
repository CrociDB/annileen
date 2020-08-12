import os
import shutil
import sys
import argparse

import tools
from tools import bcolors
import shader
import mesh
import texture
import cubemap

asset_descriptor = {
    'asset': {}
}

def build_assets(platform, shader_model, force=False):
    build_shaders(platform, shader_model, force)
    build_textures(platform, force)
    build_meshes(platform, force)
    build_cubemaps(platform, force)

    descriptor_path = tools.save_descriptor(asset_descriptor)
    print(f'\n{tools.bcolors.OKBLUE}ASSET DESCRIPTOR WRITTEN AT: {tools.bcolors.ENDC}{tools.bcolors.BOLD}{descriptor_path}{tools.bcolors.ENDC}') 

def build_shaders(platform, shader_model, force=False):
    print(f'{tools.bcolors.OKBLUE}COMPILING SHADERS{tools.bcolors.ENDC}')
    for s in shader.build_all(platform, shader_model, force):
        if s[0]:
            asset_descriptor['asset'][s[1].lower()] = {'path': s[2].replace(os.getcwd(), "."), 'type': 'shader'}

def build_textures(platform, force=False):
    print(f'{tools.bcolors.OKBLUE}COMPILING TEXTURES{tools.bcolors.ENDC}')
    for t in texture.build_all(force):
        if t[0]:
            asset_descriptor['asset'][t[1].lower()] = {'path': t[2].replace(os.getcwd(), "."), 'type': 'texture'}

def build_meshes(platform, force=False):
    print(f'{tools.bcolors.OKBLUE}COMPILING MESHES{tools.bcolors.ENDC}')
    for m in mesh.build_all(force):
        if m[0]:
            asset_descriptor['asset'][m[1].lower()] = {'path': m[2].replace(os.getcwd(), "."), 'type': 'mesh'}

def build_cubemaps(platform, force=False):
    print(f'{tools.bcolors.OKBLUE}COMPILING CUBEMAPS{tools.bcolors.ENDC}')
    for m in cubemap.build_all(force):
        if m[0]:
            asset_descriptor['asset'][m[1].lower()] = {'path': m[2].replace(os.getcwd(), "."), 'type': 'mesh'}

def ensure_build_dir():
    build_assets = os.path.join(os.getcwd(), tools.build_dir)
    origin_assets = os.path.join(os.getcwd(), tools.root_dir)

    if not os.path.isdir(build_assets):
        print(f'{tools.bcolors.OKBLUE}CREATING ASSET DIR AT {tools.bcolors.BOLD} {build_assets}{tools.bcolors.ENDC}')
        os.mkdir(build_assets)

    for d in tools.dirs:
        complete_dir = os.path.join(build_assets, tools.dirs[d])
        if not os.path.isdir(complete_dir):
            print(f' - Creating {d} dir: {tools.bcolors.UNDERLINE}{complete_dir}{tools.bcolors.ENDC}')
            os.mkdir(complete_dir)


def check_required_dir():
    root = os.path.join(os.getcwd(), tools.root_dir)
    for d in tools.dirs:
        complete_dir = os.path.join(root, tools.dirs[d])
        if not os.path.isdir(complete_dir):
            print(f'{tools.bcolors.ERROR}[ERROR]{tools.bcolors.ENDC} "{complete_dir}" does not exist.')
            return False
    return True

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=f'{bcolors.SUCCESS}Annileen Asset Tools{bcolors.ENDC}')
    parser.add_argument('-f', '--force', help='force rebuild of all assets', action='store_true')
    parser.add_argument('-p', '--platform', help='compiles the mesh specified', choices=tools.available_platforms, default='auto')
    parser.add_argument('-s', '--shader', help='shader model', choices=tools.available_shader_models, default='auto')
    args = parser.parse_args()
    

    if check_required_dir():
        ensure_build_dir()
        build_assets(args.platform, args.shader, args.force)
