cmake_minimum_required(VERSION 2.8.11)

project(foss4g-banner)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_definitions(-Wall -std=c++11)

include(GNUInstallDirs)

find_package(Qt5Widgets)
find_package(Qt5Quick)
find_package(Qt5WebKitWidgets)

add_executable(foss4g-banner WIN32 main.cpp paths.cpp talk-model.cpp)
set_source_files_properties(paths.cpp
    PROPERTIES COMPILE_DEFINITIONS "DATADIR=\"${CMAKE_INSTALL_FULL_DATADIR}\""
)
target_link_libraries(foss4g-banner Qt5::Quick)

add_executable(model-test model-test.cpp talk-model.cpp)
target_link_libraries(model-test Qt5::Network)

add_executable(web-banner-launcher web.cpp)
target_link_libraries(web-banner-launcher Qt5::Widgets Qt5::WebKitWidgets)

install(TARGETS foss4g-banner
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES main.qml Slide1.qml Slide2.qml patagonia-bg.png foss4g-ar-1.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)

add_executable(vtprepare vtprepare.cpp)
