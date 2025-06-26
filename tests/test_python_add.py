import sys
from pathlib import Path

# Add build directory to sys.path to import the compiled module
build_dir = Path(__file__).resolve().parents[1] / "build" / "src"
sys.path.insert(0, str(build_dir))

import shm_py

def test_add_from_python():
    assert shm_py.add(2, 3) == 5
