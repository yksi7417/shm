import sys
import os

# Add build directory to sys.path to import the compiled module
build_dir = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "build",
    "src",
)
sys.path.insert(0, build_dir)

import shm_py

def test_add_from_python():
    assert shm_py.add(2, 3) == 5
