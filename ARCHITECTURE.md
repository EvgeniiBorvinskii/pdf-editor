# PDF Editor - Architecture Overview

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        Application                           │
│                         (main.cpp)                           │
└───────────────────────────┬─────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                       MainWindow                             │
│  • Manages application lifecycle                            │
│  • Coordinates all components                               │
│  • Handles menu actions                                     │
└──────┬──────────────┬──────────────┬────────────────────────┘
       │              │              │
       ▼              ▼              ▼
┌──────────┐   ┌──────────┐   ┌──────────────┐
│  Custom  │   │   Edit   │   │  Animation   │
│ TitleBar │   │ Toolbar  │   │   Engine     │
│          │   │          │   │              │
│ • Drag   │   │ • Tools  │   │ • 240 FPS    │
│ • Min    │   │ • Select │   │ • Easing     │
│ • Max    │   │ • Icons  │   │ • Smooth     │
│ • Close  │   └──────────┘   └──────────────┘
└──────────┘
       │
       ▼
┌─────────────────────────────────────────────────────────────┐
│                      PDFViewer                               │
│  • OpenGL rendering                                         │
│  • Hardware acceleration                                    │
│  • Mouse/keyboard input                                     │
│  • Zoom & pan controls                                      │
└──────┬────────────────────────────┬─────────────────────────┘
       │                            │
       ▼                            ▼
┌──────────────┐            ┌──────────────┐
│ PDFDocument  │◄───────────│  PDFEditor   │
│              │            │              │
│ • Load/Save  │            │ • Edit modes │
│ • Render     │            │ • Tools      │
│ • Page mgmt  │            │ • Undo/Redo  │
└──────┬───────┘            └──────┬───────┘
       │                           │
       │                           ▼
       │              ┌────────────────────────┐
       │              │   Editing Tools        │
       │              ├────────────────────────┤
       │              │ • TextTool             │
       │              │ • HighlightTool        │
       │              │ • DrawingTool          │
       │              │ • AnnotationTool       │
       │              │ • ImageTool            │
       │              └────────────────────────┘
       │
       ▼
┌─────────────────────────────────────────────────────────────┐
│                    PDF Library Layer                         │
│  (Future: PDFium, PoDoFo, or MuPDF integration)             │
└─────────────────────────────────────────────────────────────┘
```

## Component Hierarchy

```
QApplication
└── MainWindow (QMainWindow)
    ├── CustomTitleBar (QWidget)
    │   ├── Title Label
    │   └── Window Controls
    │       ├── Minimize Button
    │       ├── Maximize Button
    │       └── Close Button
    │
    ├── EditToolbar (QWidget)
    │   └── Tool Buttons (QToolButton)
    │       ├── Select
    │       ├── Text
    │       ├── Highlight
    │       ├── Drawing
    │       ├── Shape
    │       ├── Image
    │       ├── Annotation
    │       └── Eraser
    │
    ├── PDFViewer (QOpenGLWidget)
    │   ├── PDFDocument
    │   │   └── Page Cache
    │   └── PDFEditor
    │       └── Edit Tools
    │
    └── AnimationEngine
        └── Animation Queue
```

## Data Flow

```
User Input
    │
    ▼
┌────────────┐
│ MainWindow │
└─────┬──────┘
      │
      ├──► Tool Selection ──► EditToolbar ──► PDFEditor
      │
      ├──► File Operations ──► PDFDocument
      │                            │
      │                            ├──► Load PDF
      │                            ├──► Render Pages
      │                            └──► Save Changes
      │
      └──► View Controls ──► PDFViewer
                                 │
                                 ├──► Zoom
                                 ├──► Pan
                                 └──► Render
```

## Rendering Pipeline

```
┌──────────────┐
│ PDF Document │
└──────┬───────┘
       │
       ▼
┌──────────────────┐
│  Page Renderer   │  ◄── Apply zoom/scale
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│  Cached Image    │
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ Apply Edits      │  ◄── Drawings, highlights, text
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ OpenGL Display   │  ◄── Hardware acceleration
└──────────────────┘
       │
       ▼
   [Screen]
```

## Animation System

```
AnimationEngine (240 FPS)
    │
    ├──► Frame Timer (4ms interval)
    │
    ├──► Animation Queue
    │    ├── Animation 1 (Easing: EaseInOut)
    │    ├── Animation 2 (Easing: Elastic)
    │    └── Animation N (Easing: Bounce)
    │
    ├──► Calculate Delta Time
    │
    ├──► Update All Animations
    │    └──► Apply Easing Function
    │
    └──► Emit frameRendered() signal
         │
         └──► Update UI Components
```

## Event Flow

```
Mouse/Keyboard Event
        │
        ▼
    PDFViewer
        │
        ├──► Wheel Event ──► Zoom or Page Change
        │
        ├──► Mouse Press ──► Begin Edit
        │         │
        │         └──► PDFEditor.beginEdit()
        │
        ├──► Mouse Move ──► Continue Edit
        │         │
        │         └──► PDFEditor.continueEdit()
        │
        └──► Mouse Release ──► End Edit
                  │
                  └──► PDFEditor.endEdit()
                        │
                        └──► Update Document
```

## Thread Architecture

```
Main Thread (UI)
    │
    ├──► Event Loop
    ├──► Animation Engine
    ├──► OpenGL Rendering
    └──► User Input Handling

Background Threads (Future Enhancement)
    │
    ├──► PDF Page Rendering
    ├──► File I/O Operations
    └──► Image Processing
```

## Memory Management

```
MainWindow
    │
    ├──► std::unique_ptr<AnimationEngine>
    │
    ├──► CustomTitleBar* (Qt parent-child ownership)
    │
    ├──► EditToolbar* (Qt parent-child ownership)
    │
    └──► PDFViewer* (Qt parent-child ownership)
          │
          ├──► std::unique_ptr<PDFDocument>
          └──► std::unique_ptr<PDFEditor>
```

## Design Patterns

### 1. PIMPL (Private Implementation)
```cpp
class PDFDocument {
    class Private;
    std::unique_ptr<Private> d;  // Hides implementation
};
```

### 2. Observer (Qt Signals/Slots)
```cpp
connect(toolbar, &EditToolbar::toolChanged,
        viewer, &PDFViewer::setCurrentTool);
```

### 3. Strategy (Tool System)
```cpp
PDFEditor::setEditMode(EditMode::Drawing);
// Different behavior based on selected tool
```

### 4. Factory (Tool Creation)
```cpp
QToolButton* createToolButton(Tool type);
```

## Performance Optimizations

### 1. Rendering
- OpenGL hardware acceleration
- VSync disabled for higher FPS
- MSAA 4x anti-aliasing
- Lazy updates (only render when needed)

### 2. Memory
- Page caching
- Smart pointers
- Efficient resource management
- On-demand loading

### 3. CPU
- Delta time calculations
- Optimized easing functions
- Minimal redraws
- Event batching

## Plugin Architecture (Future)

```
┌─────────────────────────────────────┐
│         Plugin Manager              │
└───────────┬─────────────────────────┘
            │
            ├──► Tool Plugins
            │    ├── Custom Tools
            │    └── Third-party Tools
            │
            ├──► Export Plugins
            │    ├── Image Export
            │    └── Format Converters
            │
            └──► Processing Plugins
                 ├── OCR
                 ├── AI Enhancement
                 └── Batch Operations
```

## Security Considerations

- Input validation for PDF files
- Safe rendering (prevent malicious PDFs)
- Secure file operations
- Memory bounds checking
- Exception handling

## Testing Strategy

```
Unit Tests
├── Component Tests
│   ├── AnimationEngine
│   ├── PDFDocument
│   └── Tools
│
Integration Tests
├── UI Interaction
├── File Operations
└── Rendering Pipeline
│
Performance Tests
├── FPS Benchmarks
├── Memory Usage
└── Load Testing
```

---

This architecture provides:
- **Modularity** - Easy to extend and modify
- **Performance** - Optimized at every level
- **Maintainability** - Clean separation of concerns
- **Scalability** - Ready for future enhancements
