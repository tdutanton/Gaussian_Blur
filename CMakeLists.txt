cmake_minimum_required(VERSION 3.5...3.10)

project(gaussian_blur VERSION 0.1 LANGUAGES CXX)

if(WIN32 AND DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
   set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
       CACHE STRING "")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.12/5.12.12/msvc2017") - если не собирается - указать путь к Qt
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        resource.qrc

        src/modaldialog.h
        src/modaldialog.cpp

        src/radiuscontrolpanel.h
        src/radiuscontrolpanel.cpp

        src/onedkernel.h
        src/onedkernel.cpp

        src/bluredimage.h
        src/bluredimage.cpp

        src/common_utils.h
        src/common_utils.cpp

        src/progressbarblur.h
        src/progressbarblur.cpp
)


add_executable(gaussian_blur ${PROJECT_SOURCES})


target_link_libraries(gaussian_blur PRIVATE Qt5::Widgets)

set_target_properties(gaussian_blur PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gaussian_blur)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Ob2 /Oi /Ot")
  # Включает макрос __cplusplus, чтобы сообщить о поддерживаемом стандарте 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # using GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") # using Clang
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-var-template -Wno-switch")

  if (CMAKE_CXX_COMPILER MATCHES ".+clang-cl.+") # using Clang-CL
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Ob2 /Oi /Ot")
    target_link_libraries(${PROJECT_NAME} libomp.lib)
  else() # using Clang GNU
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif()
endif()
