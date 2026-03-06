image := "annileen-dev"
podman_run := "podman run --rm -v \"$(pwd):/workspace\" -w /workspace " + image

setup:
    podman build -t {{image}} -f Containerfile .

build config="release":
    {{podman_run}} bash -c "premake5 gmake2 && make -C build/gmake2 config={{config}}_x86_64"

example name:
    #!/usr/bin/env bash
    set -euo pipefail

    # find the most recently modified configuration directory
    latest_config=$(ls -dt build/gmake2/bin/x86_64/*/ 2>/dev/null | head -n1 | xargs basename)

    if [ -z "$latest_config" ]; then
        echo "No build found. Please run 'just build' first."
        exit 1
    fi

    binary="build/gmake2/bin/x86_64/${latest_config}/example-{{name}}"

    if [ ! -f "$binary" ]; then
        echo "Example binary not found: $binary"
        echo "Available configs: $(ls build/gmake2/bin/x86_64/ 2>/dev/null || echo 'none')"
        exit 1
    fi

    echo "Running example-{{name}} from ${latest_config} build..."
    "$binary"

clean:
    rm -rf build/

shell:
    podman run --rm -it -v "$(pwd):/workspace" -w /workspace {{image}} bash
