# Quick Qt Installation and Build Script for PDF Editor
# Run this script as Administrator

Write-Host "=== PDF Editor - Quick Setup ===" -ForegroundColor Cyan
Write-Host ""

# Step 1: Install aqt (Another Qt Installer) via pip
Write-Host "[1/4] Installing aqt (Qt installer tool)..." -ForegroundColor Yellow

# Check if Python is installed
$pythonVersion = python --version 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Python is not installed!" -ForegroundColor Red
    Write-Host "Please install Python from: https://www.python.org/downloads/" -ForegroundColor Yellow
    Write-Host "After installation, run this script again." -ForegroundColor Yellow
    pause
    exit 1
}

Write-Host "Python found: $pythonVersion" -ForegroundColor Green

# Install aqt
Write-Host "Installing aqt..." -ForegroundColor Cyan
pip install aqt

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Failed to install aqt!" -ForegroundColor Red
    pause
    exit 1
}

# Step 2: Install Qt 6.5 LTS
Write-Host ""
Write-Host "[2/4] Installing Qt 6.5.3 LTS..." -ForegroundColor Yellow
Write-Host "This may take 10-15 minutes..." -ForegroundColor Cyan

$qtPath = "$env:USERPROFILE\Qt"
New-Item -ItemType Directory -Force -Path $qtPath | Out-Null

aqt install-qt windows desktop 6.5.3 win64_msvc2019_64 -O $qtPath

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Failed to install Qt!" -ForegroundColor Red
    pause
    exit 1
}

Write-Host "Qt installed successfully!" -ForegroundColor Green

# Step 3: Configure build
Write-Host ""
Write-Host "[3/4] Configuring build..." -ForegroundColor Yellow

$projectPath = "c:\Users\KeyWest\Desktop\PROJECTS\PDF Editor"
$buildPath = "$projectPath\build"

# Remove old build directory if exists
if (Test-Path $buildPath) {
    Write-Host "Removing old build directory..." -ForegroundColor Cyan
    Remove-Item -Recurse -Force $buildPath
}

New-Item -ItemType Directory -Force -Path $buildPath | Out-Null
Set-Location $buildPath

# Set Qt path
$env:CMAKE_PREFIX_PATH = "$qtPath\6.5.3\msvc2019_64"
$env:Path = "$qtPath\6.5.3\msvc2019_64\bin;$env:Path"

Write-Host "Qt Path: $env:CMAKE_PREFIX_PATH" -ForegroundColor Cyan

# Configure with CMake
Write-Host "Running CMake configure..." -ForegroundColor Cyan
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="$qtPath\6.5.3\msvc2019_64"

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Make sure you have:" -ForegroundColor Yellow
    Write-Host "- Visual Studio 2019 or 2022 with C++ support" -ForegroundColor Yellow
    Write-Host "- CMake 3.20 or newer" -ForegroundColor Yellow
    pause
    exit 1
}

# Step 4: Build
Write-Host ""
Write-Host "[4/4] Building project..." -ForegroundColor Yellow
Write-Host "This may take 5-10 minutes..." -ForegroundColor Cyan

cmake --build . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    pause
    exit 1
}

# Deploy Qt dependencies
Write-Host ""
Write-Host "Deploying Qt dependencies..." -ForegroundColor Cyan
Set-Location Release
& "$qtPath\6.5.3\msvc2019_64\bin\windeployqt.exe" PDFEditor.exe

# Success!
Write-Host ""
Write-Host "==================================" -ForegroundColor Green
Write-Host "SUCCESS! PDF Editor is ready!" -ForegroundColor Green
Write-Host "==================================" -ForegroundColor Green
Write-Host ""
Write-Host "Executable location:" -ForegroundColor Cyan
Write-Host "$buildPath\Release\PDFEditor.exe" -ForegroundColor Yellow
Write-Host ""
Write-Host "To run the application:" -ForegroundColor Cyan
Write-Host "cd '$buildPath\Release'" -ForegroundColor Yellow
Write-Host ".\PDFEditor.exe" -ForegroundColor Yellow
Write-Host ""

# Ask to run
$run = Read-Host "Do you want to run PDF Editor now? (y/n)"
if ($run -eq "y" -or $run -eq "Y") {
    Start-Process "PDFEditor.exe"
}

Write-Host ""
Write-Host "Setup complete!" -ForegroundColor Green
pause
