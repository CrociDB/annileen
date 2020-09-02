import os
import argparse
import glob
import ntpath
from shutil import copyfile
from functools import reduce

import tools
from tools import bcolors

models_path = os.path.join(os.getcwd(), tools.root_dir, tools.dirs['models'])
models_build_path = os.path.join(os.getcwd(), tools.build_dir, tools.dirs['models'])

tools_dir = os.path.dirname(os.path.realpath(__file__))
bgfx_tools_dir = os.path.join(tools_dir, 'bgfx-tools', tools.get_platform())
bgfx_geometryc = os.path.join(bgfx_tools_dir, 'geometryc')
bgfx_geometryv = os.path.join(bgfx_tools_dir, 'geometryv')

def build_mesh(meshfile, dest, options, force=False):
    print(f" - Compiling {bcolors.UNDERLINE}'{meshfile}'{bcolors.ENDC}")
    output_file = os.path.join(dest, tools.path_leaf(meshfile.split('.')[0]) + '.mesh')

    if not force and not tools.check_should_build(output_file, meshfile): 
        print(f" {bcolors.WARNING}- SKIPPED{bcolors.ENDC}")
        return True, tools.path_leaf(meshfile), output_file

    # command = "%s -f %s -o %s -s 1 --packnormal 0 --packuv 0" % (
    #     bgfx_geometryc,
    #     meshfile,
    #     output_file,
    # )

    # success = False
    # if not os.system(command):
    #     success = True
    #     print(f" {bcolors.SUCCESS}- Mesh compiled: {output_file}{bcolors.ENDC}")

    success = copyfile(meshfile, output_file)
    if success:
        print(f" {bcolors.SUCCESS}- Mesh copied: {output_file}{bcolors.ENDC}")

    return success, tools.path_leaf(meshfile), output_file


def _build_mesh(meshname, options):
    filepath = glob.glob(os.path.join(models_path, meshname), recursive=True)
    if filepath != None and len(filepath) == 1:
        build_mesh(filepath[0], models_build_path, options)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{meshname}' not found.")

def build_all(force=False):
    meshes = reduce(lambda x, y : x + y, [glob.glob(os.path.join(models_path, "**", "*." + filetype), recursive=True) for filetype in tools.mesh_types])
    return [build_mesh(meshfile, models_build_path, "", force) for meshfile in meshes]

def view_mesh(meshname):
    filepath = glob.glob(os.path.join(models_build_path, meshname.split('.')[0] + '.*'), recursive=True)
    if filepath != None and len(filepath) > 0: 
        command = "%s %s" % (
            bgfx_geometryv,
            filepath[0],
        )
        os.system(command)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{meshname}' not found.")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=f'{bcolors.SUCCESS}Annileen Mesh Tools{bcolors.ENDC}')
    parser.add_argument('-m', '--mesh', nargs='*', help='compiles the mesh specified')
    parser.add_argument('-a', '--all', action='store_true', help='compiles all the available meshes')
    parser.add_argument('-f', '--force', help='force rebuild', action='store_true')
    parser.add_argument('-v', '--view', nargs=1, help='view the specified mesh')
    args = parser.parse_args()

    if args.all:
        build_all(args.force)
    elif args.mesh != None:
        _build_mesh(args.mesh[0], " ".join(args.mesh[1:]), args.force)
    elif args.view != None:
        view_mesh(args.view[0])
    else:
        parser.print_help()