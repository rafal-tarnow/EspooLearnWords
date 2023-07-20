cmake_minimum_required(VERSION 3.16)

project(B0002_Simulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appB0002_Simulator
    main.cpp
    tcp_client.hpp
    tcp_client.cpp
)

qt_add_qml_module(appB0002_Simulator
    URI B0002_Simulator
    VERSION 1.0
    QML_FILES Main.qml
)

set_target_properties(appB0002_Simulator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appB0002_Simulator
    PRIVATE Qt6::Quick
)

install(TARGETS appB0002_Simulator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
