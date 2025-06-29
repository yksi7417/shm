import sys
import os
import shm_py  # noqa: E402


def test_add_from_python():
    assert shm_py.add(2, 3) == 5
