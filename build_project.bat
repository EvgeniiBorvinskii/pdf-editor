@echo off
echo ===================================
echo PDF Editor - Build Project
echo ===================================
echo.

set QT_PATH=%USERPROFILE%\Qt\6.5.3\msvc2019_64
set PROJECT_PATH=%~dp0
set BUILD_PATH=%PROJECT_PATH%build

echo Qt Path: %QT_PATH%
echo Project Path: %PROJECT_PATH%
echo Build Path: %BUILD_PATH%
echo.

if not exist "%QT_PATH%" (
    echo ERROR: Qt not found at %QT_PATH%
    echo Please run install_dependencies.bat first!
    pause
    exit /b 1
)

echo Cleaning old build...
if exist "%BUILD_PATH%" rmdir /s /q "%BUILD_PATH%"
mkdir "%BUILD_PATH%"

cd "%BUILD_PATH%"

echo.
echo [1/3] Running CMake configure...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%"

if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed!
    echo.
    echo Make sure you have Visual Studio 2019 or 2022 installed with C++ support.
    echo Download from: https://visualstudio.microsoft.com/downloads/
    echo.
    pause
    exit /b 1
)

echo.
echo [2/3] Building project (Release mode)...
cmake --build . --config Release

if errorlevel 1 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo [3/3] Deploying Qt dependencies...
cd Release
"%QT_PATH%\bin\windeployqt.exe" PDFEditor.exe

echo.
echo ===================================
echo SUCCESS! PDF Editor is ready!
echo ===================================
echo.
echo Executable: %BUILD_PATH%\Release\PDFEditor.exe
echo.
echo Starting application...
start PDFEditor.exe

pause
