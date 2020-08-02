import os
import argparse
import glob
import ntpath
from functools import reduce
from PIL import Image

import tools
from tools import bcolors

cubemap_path = os.path.join(os.getcwd(), tools.root_dir, tools.dirs['cubemaps'])
texture_path = os.path.join(os.getcwd(), tools.root_dir, tools.dirs['textures'])
cubemap_build_path = os.path.join(os.getcwd(), tools.build_dir, tools.dirs['cubemaps'])

tools_dir = os.path.dirname(os.path.realpath(__file__))
bgfx_tools_dir = os.path.join(tools_dir, 'bgfx-tools', tools.get_platform())
bgfx_texturec = os.path.join(bgfx_tools_dir, 'texturec')
bgfx_texturev = os.path.join(bgfx_tools_dir, 'texturev')

def build_cubemap(cubemapfile, dest, options):
    print(f" - Compiling {bcolors.UNDERLINE}'{cubemapfile}'{bcolors.ENDC}")
    output_file = os.path.join(dest, tools.path_leaf(cubemapfile.split('.')[0]) + '.toml')
    output_texture_file = os.path.join(dest, tools.path_leaf(cubemapfile.split('.')[0]) + '.dds')

    cubemap = tools.load_asset_descriptor(cubemapfile, None)
    if cubemap == None:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{cubemap}' not found.")
        return (False, None, None)

    strip = False
    tmp = ""
    if cubemap['cubemap']['sides'] != None:
        tmp = os.path.join(dest, 'skybox-tmp.png')
        if _build_strip_texture(cubemap['cubemap']['sides'], tmp):
            strip = True

    strip_cmd = "--strip" if strip else ''

    command = "%s -f %s -o %s -t ETC2 %s" % (
        bgfx_texturec,
        tmp,
        output_texture_file,
        strip_cmd
    )

    success = False
    if not os.system(command):
        success = True
        os.remove(tmp)
        cubemap['cubemap']['sides'] = None
        cubemap['cubemap']['strip_file'] = output_texture_file
        print(f" {bcolors.SUCCESS}- Cubemap compiled: {output_file}{bcolors.ENDC}")
        tools.save_built_asset_descriptor(output_file, cubemap)

    return success, tools.path_leaf(cubemapfile), output_file

def _build_strip_texture(sides, tmp_filepah):
    order = ['front', 'back', 'up', 'down', 'right', 'left']
    images = []
    for i in order:
        f = os.path.join(texture_path, "**", sides[i])
        filepath = glob.glob(f, recursive=True)
        if filepath == None or len(filepath) != 1:
            print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{sides[i]}' not found.")
            return False

        images.append(Image.open(filepath[0]))

    w = images[0].width
    h = images[0].height
    dst = Image.new('RGB', (w * 6, h))
    i = 0
    for img in images: 
        dst.paste(img, (i * w, 0))
        i+=1

    dst.save(tmp_filepah)
    return True

def _build_cubemap(cubemap, options):
    filepath = glob.glob(os.path.join(cubemap_path, cubemap), recursive=True)
    if filepath != None and len(filepath) == 1:
     build_cubemap(filepath[0], cubemap_build_path, options)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{cubemap}' not found.")

def build_all():
    cubemaps = reduce(lambda x, y : x + y, [glob.glob(os.path.join(cubemap_path, "**", "*." + filetype), recursive=True) for filetype in tools.cubemap_types])
    print(cubemaps)
    return [build_cubemap(cubemapfile, cubemap_build_path, "") for cubemapfile in cubemaps]

def view_cubemap(cubemap):
    filepath = glob.glob(os.path.join(cubemap_build_path, "**", cubemap.split('.')[0] + '.*'), recursive=True)
    if filepath != None and len(filepath) > 0: 
        cubemap = tools.load_asset_descriptor(filepath[0], None)
        command = "%s %s" % (
            bgfx_texturev,
            cubemap['cubemap']['strip_file'],
        )
        os.system(command)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{cubemap}' not found.")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=f'{bcolors.SUCCESS}Annileen Cubemap Tools{bcolors.ENDC}')
    parser.add_argument('-c', '--cubemap', nargs='*', help='compiles the cubemap specified')
    parser.add_argument('-a', '--all', action='store_true', help='compiles all the available cubemaps')
    parser.add_argument('-v', '--view', nargs=1, help='view the specified cubemap')
    args = parser.parse_args()

    if args.all:
        build_all()
    elif args.cubemap != None:
        _build_cubemap(args.cubemap[0], " ".join(args.cubemap[1:]))
    elif args.view != None:
        view_cubemap(args.view[0])
    else:
        parser.print_help()