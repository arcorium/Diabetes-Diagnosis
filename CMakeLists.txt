cmake_minimum_required(VERSION 3.28)
project(cbr)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
  Core
  Gui
  Widgets
  Network
  Qml
  REQUIRED)

add_executable(cbr src/main.cpp
  src/ui/opening.cpp
  src/ui/opening.h
  src/ui/opening.ui
  src/ui/main_window.cpp
  src/ui/main_window.h
  src/ui/main_window.ui
  src/ui/diagnosis_result.cpp
  src/ui/diagnosis_result.h
  src/ui/diagnosis_result.ui
  src/bmi.h
  src/util.h
  src/message/protocol.h
  src/message/payload.h
  src/user.h
)
target_link_libraries(cbr
  Qt::Core
  Qt::Gui
  Qt::Widgets
  Qt::Network
)

target_include_directories(cbr PRIVATE src)