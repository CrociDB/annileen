import os
import argparse
import glob
import ntpath
from functools import reduce

import tools
from tools import bcolors

texture_path = os.path.join(os.getcwd(), tools.root_dir, tools.dirs['textures'])
texture_build_path = os.path.join(os.getcwd(), tools.build_dir, tools.dirs['textures'])

tools_dir = os.path.dirname(os.path.realpath(__file__))
bgfx_tools_dir = os.path.join(tools_dir, 'bgfx-tools', tools.get_platform())
bgfx_texturec = os.path.join(bgfx_tools_dir, 'texturec')
bgfx_texturev = os.path.join(bgfx_tools_dir, 'texturev')

def build_texture(texturefile, dest, options, force=False):
    print(f" - Compiling {bcolors.UNDERLINE}'{texturefile}'{bcolors.ENDC}")
    output_file = os.path.join(dest, tools.path_leaf(texturefile.split('.')[0]) + '.dds')

    descriptor_filename, descriptor = tools.load_asset_descriptor(texturefile, tools.texture_descriptor_schema)

    if not force and not tools.check_should_build(output_file, texturefile, descriptor_filename): 
        print(f" {bcolors.WARNING}- SKIPPED{bcolors.ENDC}")
        return True, tools.path_leaf(texturefile), output_file

    d_normalmap = 'normalmap' in descriptor and descriptor['normalmap']
    d_gamma_linear = 'gamma' in descriptor and descriptor['gamma'] == 'linear'

    mipmap = '-m 1' if descriptor['mipmap'] else ''
    gamma_linear = '--linear' if d_normalmap or d_gamma_linear else ''
    normalmap = '-n' if d_normalmap else ''

    command = "%s -f %s -o %s %s -t ETC2 %s %s" % (
        bgfx_texturec,
        texturefile,
        output_file,
        mipmap,
        gamma_linear,
        normalmap
    )

    #print(command)

    success = False
    if not os.system(command):
        success = True
        print(f" {bcolors.SUCCESS}- Texture compiled: {output_file}{bcolors.ENDC}")
        tools.save_built_asset_descriptor(output_file, descriptor)

    return success, tools.path_leaf(texturefile), output_file


def _build_texture(meshname, options, force=False):
    filepath = glob.glob(os.path.join(texture_path, meshname), recursive=True)
    if filepath != None and len(filepath) == 1:
        build_texture(filepath[0], texture_build_path, options, force)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{meshname}' not found.")

def build_all(force=False):
    textures = reduce(lambda x, y : x + y, [glob.glob(os.path.join(texture_path, "**", "*." + filetype), recursive=True) for filetype in tools.texture_types])
    return [build_texture(texturefile, texture_build_path, "", force) for texturefile in textures]

def view_texture(meshname):
    filepath = glob.glob(os.path.join(texture_build_path, meshname.split('.')[0] + '.*'), recursive=True)
    if filepath != None and len(filepath) > 0: 
        command = "%s %s" % (
            bgfx_texturev,
            filepath[0],
        )
        os.system(command)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{meshname}' not found.")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=f'{bcolors.SUCCESS}Annileen Texture Tools{bcolors.ENDC}')
    parser.add_argument('-t', '--texture', nargs='*', help='compiles the texture specified')
    parser.add_argument('-a', '--all', action='store_true', help='compiles all the available textures')
    parser.add_argument('-f', '--force', action='store_true', help='force compiling all textures')
    parser.add_argument('-v', '--view', nargs=1, help='view the specified texture')
    args = parser.parse_args()

    if args.all:
        build_all(args.force)
    elif args.texture != None:
        _build_texture(args.texture[0], " ".join(args.texture[1:]))
    elif args.view != None:
        view_texture(args.view[0])
    else:
        parser.print_help()