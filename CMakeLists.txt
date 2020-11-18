cmake_minimum_required(VERSION 3.5)

project(gt_soft_test_task LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(gt_soft_test_task
main.cpp
qml.qrc

client.h
client.cpp

model.h
model.cpp
)

target_compile_definitions(gt_soft_test_task
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(gt_soft_test_task
  PRIVATE Qt5::Core Qt5::Quick)
