cmake_minimum_required(VERSION 3.16)
project(B0002_Simulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)







find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2)

qt_standard_project_setup()

qt_add_executable(appB0002_Simulator WIN32 MACOSX_BUNDLE
    main.cpp
    tcp_client.hpp
    tcp_client.cpp
)

qt_add_qml_module(appB0002_Simulator
    URI B0002_Simulator
    VERSION 1.0
    QML_FILES
    "+Material/ToolBar.qml"
    "Main.qml"
    "ToolBar.qml"
    "pages/ButtonPage.qml"
    "pages/TCPTerminalPage.qml"
    "pages/UDPTerminalPage.qml"
    "pages/ScrollablePage.qml"
    RESOURCES
    "icons/gallery/20x20/back.png"
    "icons/gallery/20x20/drawer.png"
    "icons/gallery/20x20/menu.png"
    "icons/gallery/20x20@2/back.png"
    "icons/gallery/20x20@2/drawer.png"
    "icons/gallery/20x20@2/menu.png"
    "icons/gallery/20x20@3/back.png"
    "icons/gallery/20x20@3/drawer.png"
    "icons/gallery/20x20@3/menu.png"
    "icons/gallery/20x20@4/back.png"
    "icons/gallery/20x20@4/drawer.png"
    "icons/gallery/20x20@4/menu.png"
    "icons/gallery/index.theme"
    "images/arrow.png"
    "images/arrow@2x.png"
    "images/arrow@3x.png"
    "images/arrow@4x.png"
    "images/arrows.png"
    "images/arrows@2x.png"
    "images/arrows@3x.png"
    "images/arrows@4x.png"
    "images/qt-logo.png"
    "images/qt-logo@2x.png"
    "images/qt-logo@3x.png"
    "images/qt-logo@4x.png"
    "qmldir"
)

set_target_properties(appB0002_Simulator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appB0002_Simulator PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2
)

if(UNIX AND NOT APPLE AND CMAKE_CROSSCOMPILING)
    find_package(Qt6 REQUIRED COMPONENTS QuickTemplates2)

    # Work around QTBUG-86533
    target_link_libraries(galleryexample PRIVATE Qt::QuickTemplates2)
endif()

install(TARGETS appB0002_Simulator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
