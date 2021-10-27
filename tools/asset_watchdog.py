import os
import time
import ntpath
import glob

from watchdog.events import FileSystemEventHandler

import asset_tools
import tools
from tools import bcolors
import shader
import mesh
import texture
import cubemap
import font

def get_original_file(file):
    head, tail = ntpath.split(file)
    file_without_extension = os.path.join(head, tools.path_leaf(tail.split('.')[0]))
    filepath = glob.glob(file_without_extension + '.*')
    if filepath != None and len(filepath) > 0: 
        for f in filepath:
            ext = f.split('.')[1]
            if ext != 'toml': return f, ext


    return file, 'toml'
    

def reimport_file(file):
    # first detect if it's a descriptor file
    _, pure_filename = ntpath.split(file)
    extension = tools.path_leaf(pure_filename.split('.')[1])

    if pure_filename == tools.settings_file:
        print(f'   reimporting settings...')
        renderer_changed, renderer = tools.import_settings()

        if renderer_changed:
            print(f'   renderer type changed, reimporting everything...')
            asset_tools.build_assets(tools.get_platform(), tools.get_shadermodel_by_renderer(renderer), True)

        return

    if extension == 'toml':
        file, extension = get_original_file(file)

    asset = pure_filename.split('.')[0] + "." + extension

    ext = extension.lower()

    # now find what time of asset that is
    if ext in tools.shader_types: 
        print(f'   reimporting shader...')
        if ext == 'sh':
            # TODO: find all the shaders that import the library and only recompile them
            print(f'   {bcolors.WARNING}shader library type modified, so building all shaders{bcolors.ENDC}')
            shader.build_all('auto', 'auto', True)
        else:
            shader._build_shader(asset, {}, 'auto', 'auto', True)
    elif ext in tools.mesh_types: 
        print(f'   reimporting mesh...')
        mesh._build_mesh(asset, {}, True)
    elif ext in tools.texture_types: 
        print(f'   reimporting texture...')
        texture._build_texture(asset, {}, True)
    elif ext in tools.font_types: 
        print(f'   reimporting font...')
        font._build_font(asset, {}, True)
    elif ext in tools.cubemap_types: 
        print(f'   reimporting cubemap...')
        cubemap._build_cubemap(asset, {}, True)
    else:
        print(f'   file {asset} not reimported.')


class WatchdogHandler(FileSystemEventHandler):
    last_call = 0

    @staticmethod
    def on_any_event(event):
        if time.time() - WatchdogHandler.last_call < 2.0: return
        WatchdogHandler.last_call = time.time()

        if event.is_directory:
            return None
  
        elif event.event_type == 'created':
            print(f' - file created: {bcolors.UNDERLINE}\'{event.src_path}\'{bcolors.ENDC}. it will be ignored, run a full asset import for new files')
        elif event.event_type == 'modified':
            print(f' {bcolors.HEADER}- file modified:{bcolors.HEADER} {bcolors.UNDERLINE}\'{event.src_path}\'{bcolors.ENDC}')
            reimport_file(event.src_path)
    
        print('\n')
