import os
import sys

dirs = {
    "shaders": "shaders",
    "textures": "textures",
    "models": "models",
}

root_dir = "assets"
build_dir = "build_assets"

def build_assets():
    ensure_build_dir()

def ensure_build_dir():
    build_assets = os.path.join('.', build_dir)
    if not os.path.isdir(build_assets):
        os.mkdir(build_assets)

    for d in dirs:
        complete_dir = os.path.join(build_assets, dirs[d])
        print(complete_dir)
        if not os.path.isdir(complete_dir):
            os.mkdir(complete_dir)

def check_required_dir():
    root = os.path.join('.', root_dir)
    for d in dirs:
        complete_dir = os.path.join(root, dirs[d])
        if not os.path.isdir(complete_dir):
            print('ERROR: "%s" does not exist.' % complete_dir)
            return False
    return True
    
if __name__ == '__main__':
    if check_required_dir():
        build_assets()
