# Properties->C/C++->General->Additional Include Directories
include_directories ("include" ".")

# Set Properties->General->Configuration Type to Dynamic Library(.dll)
# Creates gtest.dll with the listed sources collected in the variables
# Also adds sources to the Solution Explorer
add_library (gtest STATIC "src/gtest-all.cc")

# Creates folder "libraries" and adds target project (math.vcproj)
# set_property(TARGET gtest PROPERTY FOLDER "libraries")

# Adds logic to INSTALL.vcproj to copy gtest.dll to destination directory
#install (TARGETS gtest RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)