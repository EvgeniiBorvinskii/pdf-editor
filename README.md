# PDF Editor

A modern, high-performance PDF editor with a beautiful Liquid Glass design and ultra-smooth animations.

## Features

- 🎨 **Modern Liquid Glass Design** - Dark theme with glassmorphism effects
- 🚀 **High Performance** - Optimized for 240 FPS animations
- 🖼️ **Borderless Window** - Custom title bar with integrated controls
- ✏️ **Full PDF Editing** - Text, drawings, annotations, highlights, images
- 💨 **Blazing Fast** - Multi-threaded rendering and optimized operations

## Build Requirements

- C++20 compatible compiler
- CMake 3.20+
- Qt 6.5+
- PDFium library (included)

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Usage

1. Launch the application
2. Open a PDF file using File → Open
3. Use the toolbar to select editing tools
4. Save your changes with File → Save

## Tools

- **Text Tool** - Add and edit text annotations
- **Highlight Tool** - Highlight important text
- **Drawing Tool** - Draw freehand annotations
- **Shape Tool** - Add shapes and arrows
- **Image Tool** - Insert images into PDF
- **Annotation Tool** - Add comments and notes

## Performance

- Optimized rendering engine
- Hardware-accelerated graphics
- Multi-threaded processing
- Smooth 240 FPS animations
- Low memory footprint

## License

MIT License - See LICENSE file for details
