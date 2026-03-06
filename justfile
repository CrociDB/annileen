# Build the project with premake5 and make
build config="release":
    premake5 gmake2
    cd build/gmake2 && make config={{config}}_x86_64

# Run an example (worldbuilding or cube) - uses most recently built config
example name:
    #!/usr/bin/env bash
    set -euo pipefail
    
    # Find the most recently modified configuration directory
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

# Clean build artifacts
clean:
    cd build/gmake2 && make clean
