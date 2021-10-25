import os
import argparse
import glob
import ntpath
from functools import reduce

import tools
from tools import bcolors
import shutil
from shutil import copyfile

font_path = os.path.join(os.getcwd(), tools.root_dir, tools.dirs['fonts'])
font_build_path = os.path.join(os.getcwd(), tools.build_dir, tools.dirs['fonts'])

def build_font(fontfile, dest, options, force=False):
    print(f" - Compiling {bcolors.UNDERLINE}'{fontfile}'{bcolors.ENDC}")
    output_file = os.path.join(dest, tools.path_leaf(fontfile))
    
    if not force and not tools.check_should_build(output_file, fontfile): 
        print(f" {bcolors.WARNING}- SKIPPED{bcolors.ENDC}")
        return True, tools.path_leaf(fontfile), output_file

    success = False    
    try: 
        shutil.copyfile(fontfile, output_file)
        success = True
        print(f" {bcolors.SUCCESS}- Font compiled: {output_file}{bcolors.ENDC}")
 
    # If source and destination are same 
    except shutil.SameFileError: 
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} Font source and destination represents the same file.") 
    
    # If destination is a directory. 
    except IsADirectoryError: 
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} Destination is a directory.") 
    
    # If there is any permission issue 
    except PermissionError: 
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} Permission denied.") 
    
    # For other errors 
    except: 
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} Error occurred while copying file: {output_file}") 

    return success, tools.path_leaf(fontfile), output_file


def _build_font(fontname, options, force=False):
    filepath = glob.glob(os.path.join(font_path, fontname), recursive=True)
    if filepath != None and len(filepath) == 1:
        build_font(filepath[0], font_build_path, options, force)
    else:
        print(f"{bcolors.ERROR}[ERROR]{bcolors.ENDC} File '{fontname}' not found.")

def build_all(force=False):
    fonts = reduce(lambda x, y : x + y, [glob.glob(os.path.join(font_path, "**", "*." + filetype), recursive=True) for filetype in tools.font_types])
    return [build_font(fontfile, font_build_path, "", force) for fontfile in fonts]

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=f'{bcolors.SUCCESS}Annileen Font Tools{bcolors.ENDC}')
    parser.add_argument('-f', '--font', nargs='*', help='compiles the font specified')
    parser.add_argument('-a', '--all', action='store_true', help='compiles all the available fonts')
    args = parser.parse_args()

    if args.all:
        build_all()
    elif args.font != None:
        _build_font(args.font[0], " ".join(args.font[1:]))
    else:
        parser.print_help()