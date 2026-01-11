@echo off
echo ===================================
echo PDF Editor - Quick Setup
echo ===================================
echo.
echo This will install:
echo - Python 3
echo - Qt 6 (via aqt)
echo.
pause

echo.
echo [1/3] Installing Python...
choco install python -y

echo.
echo [2/3] Refreshing environment...
refreshenv

echo.
echo [3/3] Installing Qt via aqt...
python -m pip install --upgrade pip
pip install aqt

echo.
echo Installing Qt 6.5.3 LTS...
aqt install-qt windows desktop 6.5.3 win64_msvc2019_64 -O "%USERPROFILE%\Qt"

echo.
echo ===================================
echo Installation Complete!
echo ===================================
echo.
echo Qt installed to: %USERPROFILE%\Qt\6.5.3
echo.
echo Now run: build_project.bat
pause
