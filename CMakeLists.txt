cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

# Setup the project.
# Its name is defined to be the controller directory name.
get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT})

# Get C or C++ sources in the current directory (only).
file(GLOB C_SOURCES *.c)
file(GLOB CPP_SOURCES *.cpp)

set(SOURCES ${C_SOURCES} ${CPP_SOURCES}
        lib/util/vector/Vector.hpp
        lib/util/vector/Vector.cpp
        lib/util/pid/Pid.cpp
        lib/util/pid/Pid.h
        lib/webots/controller/Controller.cpp
        lib/webots/controller/Controller.h
        lib/neural_network/network/Network.hpp
        lib/neural_network/network/Network.cpp
        lib/util/matrix/Matrix.hpp
        lib/util/matrix/Matrix.cpp
        lib/neural_network/neuron/Neuron.hpp
        lib/neural_network/neuron/Neuron.cpp
        lib/neural_network/layer/Layer.hpp
        lib/neural_network/layer/Layer.cpp lib/webots/youbot/arm/Arm.cpp lib/webots/youbot/arm/Arm.hpp)

# Link with the Webots controller library.
link_directories(/usr/local/webots/lib/controller)
set(LIBRARIES /usr/local/webots/lib/controller/libController.so Controller${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_SHARED_LIBRARY_PREFIX}CppController${CMAKE_SHARED_LIBRARY_SUFFIX})
include_directories(/usr/local/webots/lib/controller /usr/local/webots/include/controller/c /usr/local/webots/include/controller/cpp)

# Setup the target executable.
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

# Copy the target executable at the right location.
add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${PROJECT} ${CMAKE_SOURCE_DIR})