import sys
import os

# PYTHONPATH is set by CTest to point to the built module
build_dir = os.environ.get("PYTHONPATH")
if not build_dir:
    # Try to find the build directory automatically
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    
    # Check for different build configurations
    possible_paths = [
        os.path.join(base_dir, "build", "src"),  # Linux/Make build
        os.path.join(base_dir, "build_win", "src", "Release"),  # Win Release
        os.path.join(base_dir, "build_win", "src", "Debug"),    # Win Debug
        os.path.join(base_dir, "build_win", "src"),             # Win general
    ]
    
    for path in possible_paths:
        if os.path.exists(path):
            # Check if the module file exists in this directory
            module_files = [
                f for f in os.listdir(path)
                if f.startswith('shm_py') and (
                    f.endswith('.so') or
                    f.endswith('.dll') or
                    f.endswith('.pyd')
                )
            ]
            if module_files:
                build_dir = path
                break
    
    if not build_dir:
        raise ImportError(
            "Could not find shm_py module in any expected build directory"
        )

sys.path.insert(0, build_dir)

import shm_py  # noqa: E402


def test_add_from_python():
    assert shm_py.add(2, 3) == 5
