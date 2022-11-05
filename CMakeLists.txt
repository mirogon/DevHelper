cmake_minimum_required(VERSION 3.10)

project(DevHelper LANGUAGES CXX)

#set(CMAKE_BINARY_DIR build)

include(build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(DevHelper src/main.cpp src/Puller.h src/Pusher.h src/global.h "src/Pusher.cpp")
target_include_directories(DevHelper PUBLIC F:/DEV/C++/Libraries/M1Libraries2/include)
target_link_libraries(DevHelper PUBLIC ${CONAN_LIBS})