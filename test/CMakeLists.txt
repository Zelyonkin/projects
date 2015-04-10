# Collect sources into the variable SRC_FILES
file (GLOB SRC_FILES "*.h" "*.cpp")

	  # Properties->C/C++->General->Additional Include Directories
include_directories ("${PROJECT_SOURCE_DIR}/gtest/include")

# Set Properties->General->Configuration Type to Application(.exe)
# Creates main.exe with the listed sources (main.cpp)
# Adds sources to the Solution Explorer
add_executable (main ${SRC_FILES})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (main gtest)

# Creates a folder "executables" and adds target 
# project (main.vcproj) under it
#set_property(TARGET main PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy main.exe to destination directory
install (TARGETS main
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)