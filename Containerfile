FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    # Build essentials
    build-essential \
    make \
    curl \
    git \
    gdb \
    # premake5
    wget \
    # OpenGL / Mesa
    libglu1-mesa-dev \
    freeglut3-dev \
    mesa-common-dev \
    libglew-dev \
    libgl1-mesa-dev \
    # X11
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxinerama-dev \
    libxi-dev \
    libxrender-dev \
    libxfixes-dev \
    libxext-dev \
    libxss-dev \
    # Wayland
    libwayland-dev \
    wayland-protocols \
    libxkbcommon-dev \
    # Python
    python3 \
    python3-pip \
    # LSP / compile_commands.json generation
    bear \
    && rm -rf /var/lib/apt/lists/*

# Install premake5 from upstream binary (not in Ubuntu 24.04 repos)
RUN wget -qO /tmp/premake5.tar.gz \
    https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-linux.tar.gz \
    && tar -xzf /tmp/premake5.tar.gz -C /usr/local/bin \
    && chmod +x /usr/local/bin/premake5 \
    && rm /tmp/premake5.tar.gz

# Install Python build dependencies
RUN pip3 install --break-system-packages \
    toml \
    Pillow \
    watchdog

WORKDIR /workspace
