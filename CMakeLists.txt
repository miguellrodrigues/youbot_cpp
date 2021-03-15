cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(JSON_BuildTests OFF CACHE INTERNAL "")

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
        lib/util/pid/Pid.hpp
        lib/webots/controller/Controller.cpp
        lib/webots/controller/Controller.hpp
        lib/neural_network/network/Network.hpp
        lib/neural_network/network/Network.cpp
        lib/util/matrix/Matrix.hpp
        lib/util/matrix/Matrix.cpp
        lib/neural_network/neuron/Neuron.hpp
        lib/neural_network/neuron/Neuron.cpp
        lib/neural_network/layer/Layer.hpp
        lib/neural_network/layer/Layer.cpp
        lib/webots/youbot/arm/Arm.cpp
        lib/webots/youbot/arm/Arm.hpp
        lib/webots/youbot/base/Base.cpp
        lib/webots/youbot/base/Base.hpp
        lib/webots/youbot/gripper/Gripper.cpp
        lib/webots/youbot/gripper/Gripper.hpp
        lib/webots/youbot/YouBot.cpp
        lib/webots/youbot/YouBot.hpp
        lib/util/angle/Angle.cpp
        lib/util/angle/Angle.hpp
        include/Train.hpp
        include/Test.hpp lib/neural_network/cuda/matmul/MatUtil.cu lib/neural_network/cuda/matmul/MatUtil.cuh lib/neural_network/cuda/dev_array.h lib/util/Numbers.hpp)

add_subdirectory(nlohmann_json)

# Link with the Webots controller library.
link_directories(/usr/local/webots/lib/controller)
set(LIBRARIES /usr/local/webots/lib/controller/libController.so Controller${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_SHARED_LIBRARY_PREFIX}CppController${CMAKE_SHARED_LIBRARY_SUFFIX} nlohmann_json)
include_directories(/usr/local/webots/lib/controller /usr/local/webots/include/controller/c /usr/local/webots/include/controller/cpp)


# Setup the target executable.
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

# Copy the target executable at the right location.
add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${PROJECT} ${CMAKE_SOURCE_DIR})