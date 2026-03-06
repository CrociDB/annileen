{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "annileen-dev";

  buildInputs = with pkgs; [
    # OpenGL
    libGLU
    freeglut
    glew
    libglvnd
    mesa
    mesa.drivers

    # X11
    xorg.libX11
    xorg.libXrandr
    xorg.libXcursor
    xorg.libXinerama
    xorg.xinput
    xorg.libXi
    xorg.libXrender
    xorg.libXfixes
    xorg.libXext
    xorg.libXScrnSaver

    # Wayland
    wayland
    wayland-protocols
    libxkbcommon

    # Python
    python3
    uv

    # Build tools
    premake5
    gdb
  ];

  shellHook = ''
    export ANNILEEN_ROOT=$(pwd)

    if [ ! -d ".venv" ]; then
      uv venv .venv
    fi

    source .venv/bin/activate
    uv pip install -e tools

    echo "Welcome to annileen development shell"
  '';
}
