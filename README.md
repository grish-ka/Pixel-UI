# **pixel-ui: C Pixel Art UI Library**

A portable, shared C library built on Raylib, providing foundational graphics primitives for pixel-perfect user interface generation.

The project utilizes CMake and FetchContent to structure the **Shared Library (pixel\_ui)** and demonstrate usage via the **Example Application (pixel\_ui\_example)**.

## **1\. Repository Structure**

| File / Folder | Role |
| :---- | :---- |
| CMakeLists.txt | Primary build configuration and packaging (CPack) entry point. |
| src/ | Library source files. Defines the public API (pixel\_ui.h). |
| examples/ | Demonstrative executable, linking against pixel\_ui. |
| LICENSE | MIT License. |

## **2\. Prerequisites & Build**

Requires C Compiler (gcc or MSVC), CMake (3.15+), and Git.

### **A. Environment Setup**

| Platform | Dependencies (Install Command) |
| :---- | :---- |
| **Windows (MinGW/MSYS2)** | pacman \-S mingw-w64-ucrt-x86\_64-toolchain mingw-w64-ucrt-x86\_64-cmake git |
| **Linux (Debian/Ubuntu)** | sudo apt-get install build-essential git cmake libx11-dev libxrandr-dev libxi-dev libglu1-mesa-dev libasound2-dev libxinerama-dev libxcursor-dev |

### **B. Compilation**

Navigate to the project root and execute the standard CMake sequence.

1. **Configure:** (Includes policy fix for older dependency targets)  
   cmake \-B build \-G "MinGW Makefiles" \-DCMAKE\_POLICY\_VERSION\_MINIMUM=3.5

2. **Build:** Compiles targets (pixel\_ui.dll / libpixel\_ui.so and pixel\_ui\_example.exe).  
   cmake \--build build

3. **Run Example:**  
   ./build/pixel\_ui\_example.exe

## **3\. Installation & Packaging (CPack)**

CPack is the native CMake solution for generating installers. It uses the install() commands to define what files belong in the final package.

### **A. CMake Implementation**

The CMakeLists.txt is configured to define the library and header installation paths:

\# Install the compiled library artifact (.dll/.so)  
install(TARGETS pixel\_ui  
    EXPORT pixel\_ui\_targets  
    LIBRARY DESTINATION ${CMAKE\_INSTALL\_LIBDIR}   
    ARCHIVE DESTINATION ${CMAKE\_INSTALL\_LIBDIR}  
    PUBLIC\_HEADER DESTINATION ${CMAKE\_INSTALL\_INCLUDEDIR}/pixel\_ui  
)

\# Install the public header file  
install(FILES src/pixel\_ui.h DESTINATION ${CMAKE\_INSTALL\_INCLUDEDIR}/pixel\_ui)

\# \--- CPack Configuration \---  
set(CPACK\_PACKAGE\_NAME "PixelUI")  
\# ... other CPACK\_ variables ...  
include(CPack)

### **B. Generating Installers**

1. **Generate Install Stage:** This command copies the files to a local staging directory (install/).  
   cmake \--install build \--prefix install

2. **Generate Package:** This creates the installer using the CPack configuration.  
   cpack \--config ./build/CPackConfig.cmake

| Target Platform | Command to Build Installer | Output File | Requirements |
| :---- | :---- | :---- | :---- |
| **Linux (Debian/Ubuntu)** | cpack \-G DEB | PixelUI-1.0.0-Linux.deb | None (DEB generator is built-in) |
| **Windows (MSI)** | cpack \-G WIX | PixelUI-1.0.0-Win64.msi | **WiX Toolset** must be installed on the system. |
| **Windows (ZIP/TGZ)** | cpack \-G ZIP | PixelUI-1.0.0-Win64.zip | None (Built-in) |

