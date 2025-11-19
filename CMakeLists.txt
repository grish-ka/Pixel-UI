cmake_minimum_required(VERSION 3.15)
# Set a version for CPack
project(PixelUIProject C VERSION 1.0.0) 

include(FetchContent)
include(GNUInstallDirs) # Defines CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR

# CRITICAL FIX 1: Force Raylib to be a SHARED library (DLL)
set(BUILD_SHARED_LIBS ON)

# CRITICAL FIX 2: Use 'master' for Raylib to match Raygui
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

# 2. Get raygui
FetchContent_Declare(
    raygui
    GIT_REPOSITORY "https://github.com/raysan5/raygui.git"
    GIT_TAG master
)
FetchContent_GetProperties(raygui)
if(NOT raygui_POPULATED)
    FetchContent_Populate(raygui)
endif()

# ==========================================
# 1. Build and Install the Shared Library
# ==========================================
add_library(pixel_ui SHARED src/pixel_ui.c)
target_compile_definitions(pixel_ui PUBLIC BUILDING_PIXEL_UI) # Set public for users
target_link_libraries(pixel_ui PRIVATE raylib)
target_include_directories(pixel_ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Define the installation rules for the library files
install(TARGETS pixel_ui
    EXPORT pixel_ui_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Installs .dll/.so
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # Installs .lib/.a
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pixel_ui
)

# Install the header file so users can #include <pixel_ui/pixel_ui.h>
install(FILES src/pixel_ui.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pixel_ui)


# ==========================================
# 2. Build the Example App (Optional for library users, but useful for testing)
# ==========================================
add_executable(pixel_ui_example examples/main.c)
target_link_libraries(pixel_ui_example PRIVATE pixel_ui raylib)
target_include_directories(pixel_ui_example PRIVATE "${raygui_SOURCE_DIR}/src")

# Copy runtime dependencies (DLLs) next to the executable for easy running
add_custom_command(TARGET pixel_ui_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:raylib>
    $<TARGET_FILE:pixel_ui>
    $<TARGET_FILE_DIR:pixel_ui_example>
)


# ==========================================
# 3. CPack Installer Integration
# ==========================================

# 3A. Configure the packaging details
set(CPACK_PACKAGE_NAME "PixelUI")
set(CPACK_PACKAGE_VENDOR "GrishKaDev")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple C pixel UI rendering library using raylib.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# 3B. Setup the generators (WiX for MSI, DEB for Debian/Ubuntu)
set(CPACK_GENERATOR "DEB;ZIP;TGZ;WIX") 
set(CPACK_WIX_UPGRADE_GUID "YOUR-UNIQUE-GUID-HERE") # **IMPORTANT**: MUST be replaced with a real GUID (e.g., from an online generator)
set(CPACK_WIX_LICENSE_RTF "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# 3C. Include the CPack module (must be last)
include(CPack)