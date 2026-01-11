## Build Instructions

### Windows

#### Prerequisites
- Visual Studio 2019 or newer with C++ support
- CMake 3.20+
- Qt 6.5+ (download from https://www.qt.io/download)

#### Setup Qt
1. Install Qt 6.5 or later
2. Add Qt to your PATH or set CMAKE_PREFIX_PATH:
   ```bash
   set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64
   ```

#### Build Steps
```bash
# Clone the repository
git clone https://github.com/EvgeniiBorvinskii/PDFEditor.git
cd PDFEditor

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release

# Run
Release\PDFEditor.exe
```

### Linux

#### Prerequisites
```bash
sudo apt-get update
sudo apt-get install build-essential cmake qt6-base-dev qt6-tools-dev
```

#### Build Steps
```bash
git clone https://github.com/EvgeniiBorvinskii/PDFEditor.git
cd PDFEditor
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./PDFEditor
```

### macOS

#### Prerequisites
```bash
brew install cmake qt@6
```

#### Build Steps
```bash
git clone https://github.com/EvgeniiBorvinskii/PDFEditor.git
cd PDFEditor
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/local/opt/qt@6
make -j$(sysctl -n hw.ncpu)
./PDFEditor.app/Contents/MacOS/PDFEditor
```

## Features in Detail

### Editing Tools

1. **Select Tool (🖱)** - Select and move objects
2. **Text Tool (📝)** - Add text annotations
3. **Highlight Tool (🖍)** - Highlight text with transparent colors
4. **Drawing Tool (✏️)** - Freehand drawing
5. **Shape Tool (⬜)** - Add rectangles, circles, arrows
6. **Image Tool (🖼)** - Insert images
7. **Annotation Tool (💬)** - Add comment bubbles
8. **Eraser Tool (🧹)** - Remove annotations

### Keyboard Shortcuts

- `Ctrl+O` - Open PDF
- `Ctrl+S` - Save
- `Ctrl+Shift+S` - Save As
- `Ctrl+Z` - Undo
- `Ctrl+Y` - Redo
- `Ctrl+Q` - Quit
- `Ctrl+Mouse Wheel` - Zoom in/out
- `Middle Click + Drag` - Pan view

### Performance Features

- **Hardware Acceleration** - OpenGL rendering for smooth graphics
- **Multi-threading** - Background PDF processing
- **240 FPS Animations** - Ultra-smooth UI transitions
- **Smart Caching** - Optimized page rendering
- **Low Memory Footprint** - Efficient resource management

## Troubleshooting

### Qt Not Found
If CMake cannot find Qt, specify the Qt installation path:
```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.5.0/compiler
```

### OpenGL Issues
Make sure your graphics drivers are up to date for best performance.

### Build Errors
Ensure you have all dependencies installed and CMake 3.20+.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
