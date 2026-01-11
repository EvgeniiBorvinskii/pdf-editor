# Quick Start Guide - PDF Editor

## 🚀 Get Started in 5 Minutes

### Prerequisites Check

✅ Windows 10/11  
✅ Visual Studio 2019+ with C++ support  
✅ CMake 3.20+  
✅ Qt 6.5+

### Step 1: Install Qt

Download Qt from: https://www.qt.io/download-qt-installer

Select components:
- Qt 6.5 or later
- MSVC 2019 64-bit (or newer)
- Qt Creator (optional)

### Step 2: Clone the Repository

```bash
# Once the repository is created on GitHub
git clone https://github.com/EvgeniiBorvinskii/PDFEditor.git
cd PDFEditor
```

### Step 3: Configure Qt Path

Set the Qt installation path:

```bash
set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64
```

**Note**: Adjust the path to match your Qt installation!

### Step 4: Build the Project

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build in Release mode for best performance
cmake --build . --config Release
```

### Step 5: Run!

```bash
# From the build directory
Release\PDFEditor.exe
```

## 🎮 First Use

### Opening a PDF
1. Click **File → Open** or press `Ctrl+O`
2. Select a PDF file
3. The document will open in the viewer

### Using Tools
Click any tool icon in the left toolbar:
- 🖱 **Select** - Default cursor
- 📝 **Text** - Add text
- 🖍 **Highlight** - Highlight text
- ✏️ **Drawing** - Freehand draw
- ⬜ **Shape** - Add shapes
- 🖼 **Image** - Insert images
- 💬 **Annotation** - Add notes
- 🧹 **Eraser** - Remove items

### Navigation
- **Mouse Wheel** - Scroll pages
- **Ctrl + Wheel** - Zoom in/out
- **Middle Click + Drag** - Pan view

### Saving
- **Ctrl+S** - Quick save
- **Ctrl+Shift+S** - Save as new file

## 💡 Tips & Tricks

### Performance
- The app runs at 240 FPS for ultra-smooth animations
- Hardware acceleration is enabled by default
- Close unnecessary applications for best performance

### Keyboard Shortcuts
| Shortcut | Action |
|----------|--------|
| `Ctrl+O` | Open file |
| `Ctrl+S` | Save |
| `Ctrl+Shift+S` | Save as |
| `Ctrl+Z` | Undo (coming soon) |
| `Ctrl+Y` | Redo (coming soon) |
| `Ctrl+Q` | Quit |

### Window Controls
- **Minimize** - Click the `−` button
- **Maximize** - Click the `□` button or double-click title bar
- **Close** - Click the `✕` button

## 🛠️ Troubleshooting

### "Qt not found" Error

**Solution**: Set CMAKE_PREFIX_PATH before running cmake:
```bash
set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64
cmake .. -G "Visual Studio 17 2022" -A x64
```

### "OpenGL functions not found"

**Solution**: Update your graphics drivers from:
- NVIDIA: https://www.nvidia.com/drivers
- AMD: https://www.amd.com/support
- Intel: https://www.intel.com/content/www/us/en/download-center/home.html

### Application won't start

**Solution**: 
1. Make sure you built in **Release** mode
2. Copy Qt DLLs to the executable directory:
```bash
windeployqt Release\PDFEditor.exe
```

### Slow performance

**Solution**:
1. Close other applications
2. Update graphics drivers
3. Check Task Manager for CPU/GPU usage
4. Reduce zoom level if working with large PDFs

## 📖 Learning More

- **Full Documentation**: See `README.md`
- **Build Instructions**: See `BUILD.md`
- **Architecture**: See `ARCHITECTURE.md`
- **Project Summary**: See `PROJECT_SUMMARY.md`

## 🐛 Found a Bug?

Report issues on GitHub:
https://github.com/EvgeniiBorvinskii/PDFEditor/issues

## 🤝 Want to Contribute?

See contributing guidelines in the README.md

## 📸 Screenshots (Coming Soon)

The application features:
- Modern dark theme
- Liquid Glass design
- Smooth animations
- Clean interface
- Intuitive controls

---

**Enjoy your ultra-smooth PDF editing experience! 🎉**
