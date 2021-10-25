import os
import argparse
import glob
import ntpath
from functools import reduce

import tools
from tools import bcolors

shader_path = os.path.join(os.getcwd(), tools.root_dir, tools.dirs['shaders'])
shader_build_path = os.path.join(os.getcwd(), tools.build_dir, tools.dirs['shaders'])

varying_def = "varying.def.sc"
default_varying_def = os.path.join(shader_path, "varying.def.sc")

tools_dir = os.path.dirname(os.path.realpath(__file__))
bgfx_tools_dir = os.path.join(tools_dir, 'bgfx-tools', tools.get_platform())
bgfx_shaderc = os.path.join(bgfx_tools_dir, 'shaderc')
bgfx_source_folder = os.path.join(os. getcwd(), 'bgfx', 'src')

def build_shader(shaderfile, dest, options, platform, model, force=False):
    print(f" - Compiling {bcolors.UNDERLINE}'{shaderfile}'{bcolors.ENDC}")
    output_file = os.path.join(dest, tools.path_leaf(shaderfile))

    if not force and not tools.check_should_build(output_file, shaderfile): 
        print(f" {bcolors.WARNING}- SKIPPED{bcolors.ENDC}")
        return True, tools.path_leaf(shaderfile), output_file

    shadertype = "vertex" if shaderfile.split('.')[1] == 'vs' else "fragment"

    current_path_varying = os.path.join(ntpath.split(shaderfile)[0], varying_def)
    varying_def_path = current_path_varying if os.path.isfile(current_path_varying) else default_varying_def

    platform = tools.get_platform() if platform == 'auto' else platform
    if model.find('s_') > -1:
        model = ('p' if shadertype == 'fragment' else 'v') + model
    model = '--profile 120' if model == 'auto' else f'--profile {model}'

    command = "%s -f %s -o %s -i %s --varyingdef %s --platform %s %s --type %s" % (
        bgfx_shaderc,
        shaderfile,
        output_file,
        bgfx_source_folder,
        varying_def_path,
        platform,
        model,
        shadertype
    )

    success = False
    if not os.system(command):
        success = True
        print(f" {bcolors.SUCCESS}- Shader compiled: {output_file}{bcolors.ENDC}")

    return success, tools.path_leaf(shaderfile), output_file


def _build_shader(shadername, options, platform, model, force):
    filepath = glob.glob(os.path.join(shader_path, '**', shadername), recursive=True)
    if filepath != None and len(filepath) == 1:
        build_shader(filepath[0], shader_build_path, options, platform, model, force)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{shadername}' not found.")


def build_all(platform, model, force=False):
    # removing `SH` type because it's just a shader library
    shader_types = tools.shader_types.copy()
    shader_types.remove('sh')

    shaders = reduce(lambda x, y : x + y, [glob.glob(os.path.join(shader_path, "**", "*." + filetype), recursive=True) for filetype in shader_types])
    return [build_shader(shaderfile, shader_build_path, "", platform, model, force) for shaderfile in shaders]

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=f'{bcolors.SUCCESS}Annileen Shader Tools{bcolors.ENDC}')
    parser.add_argument('-s', '--shader', nargs='*', help='compiles the shader specified')
    parser.add_argument('-f', '--force', help='force rebuild', action='store_true')
    parser.add_argument('-a', '--all', action='store_true', help='compiles all the available shaders')
    parser.add_argument('-p', '--platform', help='compiles the mesh specified', choices=tools.available_platforms, default='auto')
    parser.add_argument('-m', '--model', help='shader model', choices=tools.available_shader_models, default='auto')
    args = parser.parse_args()

    if args.all:
        build_all(args.platform, args.model, args.force)
    elif args.shader != None:
        _build_shader(args.shader[0], " ".join(args.shader[1:]), args.platform, args.model, args.force)
    else:
        parser.print_help()