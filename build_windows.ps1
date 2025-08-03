# Windows Build Script for SHM Project
# Based on the working GitHub Actions workflow

Write-Host "=== SHM Project Windows Build Script ===" -ForegroundColor Green

# Step 1: Verify Python installation
Write-Host "`n1. Checking Python installation..." -ForegroundColor Yellow
python -c "import sys; print('Python executable:', sys.executable); print('Python version:', sys.version)"

# Step 2: Get Python executable path
$pythonExe = python -c "import sys; print(sys.executable)"
Write-Host "Using Python executable: $pythonExe" -ForegroundColor Cyan

# Step 3: Install Python dependencies
Write-Host "`n2. Installing Python test dependencies..." -ForegroundColor Yellow
python -m pip install --upgrade pip setuptools wheel
pip install pytest

# Step 4: Clean previous build (optional but recommended)
Write-Host "`n3. Cleaning previous build..." -ForegroundColor Yellow
if (Test-Path build_win) { 
    Write-Host "Removing existing build_win directory..."
    Remove-Item -Recurse -Force build_win 
}

# Step 5: Configure CMake
Write-Host "`n4. Configuring CMake..." -ForegroundColor Yellow
cmake -S . -B build_win -DCMAKE_BUILD_TYPE=Release -DPython3_EXECUTABLE="$pythonExe"

# Step 6: Build
Write-Host "`n5. Building project..." -ForegroundColor Yellow
cmake --build build_win --config Release

# Step 7: Run tests
Write-Host "`n6. Running tests..." -ForegroundColor Yellow
Push-Location build_win
ctest --output-on-failure -C Release
Pop-Location

Write-Host "`n=== Build Complete! ===" -ForegroundColor Green
