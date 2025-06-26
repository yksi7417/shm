# shm

Basic C++ project with optional Python bindings.

## Pip env 

```
### Using pip (for whichever python you like—here, we’ll use python3.12)
python3.12 -m pip install --user pipenv

### Make sure ~/.local/bin is on your PATH
export PATH="$HOME/.local/bin:$PATH"
```

## Run Tests

```
cmake -S . -B build
cmake --build build -j$(nproc)
cd build && ctest -V
cd ..; pytest -q
```

