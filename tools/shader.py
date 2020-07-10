import os
import argparse
import glob
import ntpath

import asset_tools
from asset_tools import bcolors

shader_path = os.path.join(os. getcwd(), asset_tools.root_dir, asset_tools.dirs['shaders'])
shader_build_path = os.path.join(os. getcwd(), asset_tools.build_dir, asset_tools.dirs['shaders'])

varying_def = "varying.def.sc"
default_varying_def = os.path.join(shader_path, "varying.def.sc")

tools_dir = os.path.dirname(os.path.realpath(__file__))
bgfx_tools_dir = os.path.join(tools_dir, 'bgfx-tools', asset_tools.get_platform())
bgfx_shaderc = os.path.join(bgfx_tools_dir, 'shaderc')
bgfx_source_folder = os.path.join(os. getcwd(), 'bgfx', 'src')

def build_shader(shaderfile, dest, options):
    print(f" - Compiling {bcolors.UNDERLINE}'{shaderfile}'{bcolors.ENDC}")
    output_file = os.path.join(dest, _path_leaf(shaderfile))

    shadertype = "vertex" if shaderfile.split('.')[1] == 'vs' else "fragment"

    current_path_varying = os.path.join(ntpath.split(shaderfile)[0], varying_def)
    varying_def_path = current_path_varying if os.path.isfile(current_path_varying) else default_varying_def

    command = "%s -f %s -o %s -i %s --varyingdef %s --platform %s --type %s" % (
        bgfx_shaderc,
        shaderfile,
        output_file,
        bgfx_source_folder,
        varying_def_path,
        asset_tools.get_platform(),
        shadertype
    )

    success = False
    if not os.system(command):
        success = True
        print(f" {bcolors.SUCCESS}- Shader compiled: {output_file}{bcolors.ENDC}")

    return success, output_file


def _build_shader(shadername, options):
    filepath = glob.glob(os.path.join(shader_path, shadername), recursive=True)
    if filepath != None and len(filepath) == 1:
        build_shader(filepath[0], shader_build_path, options)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{shadername}' not found.")


def build_all():
    shaders = glob.glob(os.path.join(shader_path, "*.vs")) + glob.glob(os.path.join(shader_path, "*.fs"))
    for shaderfile in shaders:
        build_shader(shaderfile, shader_build_path, "")

def _path_leaf(path):
    head, tail = ntpath.split(path)
    return tail or ntpath.basename(head)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=f'{bcolors.SUCCESS}Annileen Shader Tools{bcolors.ENDC}')
    parser.add_argument('-s', '--shader', nargs=1, help='compiles the shader specified')
    parser.add_argument('-a', '--all', action='store_true', help='compiles all the available shaders')
    args = parser.parse_args()

    if args.all:
        build_all()
    elif args.shader != None:
        _build_shader(args.shader[0], " ".join(args.shader[1:]))
    else:
        parser.print_help()