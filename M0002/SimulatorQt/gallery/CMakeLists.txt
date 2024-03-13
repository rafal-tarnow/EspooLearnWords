cmake_minimum_required(VERSION 3.16)

project(KikoBricks VERSION 0.1  LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Quick3D Scxml Test)



qt_add_executable(kikoBricks WIN32 MACOSX_BUNDLE
    main.cpp
    Backend.hpp
    Backend.cpp
    src/tcp_terminal.hpp
    src/tcp_terminal.cpp
    src/udp_terminal.hpp
    src/udp_terminal.cpp
    src/emulators/emulator_m0002.hpp
    src/emulators/emulator_m0002.cpp


    src/emulators/tcp_connection.hpp
    src/emulators/tcp_connection.cpp
    src/tests/PseudoDNSTest.cpp
    src/emulators/protocol.h
    src/emulators/protocol.cpp
    src/emulators/ProtocolStd.h
    src/emulators/ProtocolStd.cpp
    src/global_config.hpp
    src/wifi_info.hpp
    src/emulators/BrickIpFinder.hpp
    src/emulators/BrickIpFinder.cpp
    src/ObjectCounter.hpp
    src/ObjectCounter.cpp
)

qt_add_qml_module(kikoBricks
    URI KikoBricks
    VERSION 1.0
    NO_RESOURCE_TARGET_PATH

    SOURCES


    Controller.hpp
    Controller.cpp
    K0002Controller.hpp
    K0002Controller.cpp
    MyBricksManager.hpp
    MyBricksManager.cpp
    T0002Controller.hpp
    T0002Controller.cpp
    pseudo_dns.hpp
    pseudo_dns.cpp

    QML_FILES

    pages/list_view/common/BrickTitle.qml
    pages/list_view/T0002/T0002MainPage.qml
    pages/list_view/common/NetworkSettings.qml
    pages/controls/SButton.qml
    pages/list_view/common/DataLine.qml
    pages/list_view/common/BigDataLine.qml
    pages/list_view/common/BrickInfoPage.qml
    pages/list_view/common/ControllerCommon.qml
    pages/list_view/NewT0002Delegate.qml
    pages/list_view/common/AddBrickDialog.qml
    pages/list_view/common/DialogBackground.qml
    pages/list_view/common/WarningDialog.qml
    "+Material/ToolBar.qml"
    "ToolBar.qml"
    "kiko_main.qml"
    "pages/BusyIndicatorPage.qml"
    "pages/ButtonPage.qml"
    "pages/CheckBoxPage.qml"
    "pages/ComboBoxPage.qml"
    "pages/ConfigM0002Page.qml"
    "pages/DelayButtonPage.qml"
    "pages/DelegatePage.qml"
    "pages/DevicesInNetworkPage.qml"
    "pages/DialPage.qml"
    "pages/DialogPage.qml"
    "pages/EmulatorM0002Page.qml"
    "pages/FramePage.qml"
    "pages/GroupBoxPage.qml"
    "pages/PageIndicatorPage.qml"
    "pages/ProgressBarPage.qml"
    "pages/RadioButtonPage.qml"
    "pages/RangeSliderPage.qml"
    "pages/ScrollBarPage.qml"
    "pages/ScrollIndicatorPage.qml"
    "pages/ScrollablePage.qml"
    "pages/SliderPage.qml"
    "pages/SpinBoxPage.qml"
    "pages/StackViewPage.qml"
    "pages/SwipeViewPage.qml"
    "pages/SwitchPage.qml"
    "pages/TabBarPage.qml"
    "pages/TCPTerminalPage.qml"
    "pages/TextAreaPage.qml"
    "pages/TextFieldPage.qml"
    "pages/ToolTipPage.qml"
    "pages/TumblerPage.qml"
    "pages/UDPTerminalPage.qml"
    "pages/my_bricks/MyBricksPage.qml"
    "pages/my_bricks/BrickDelegate.qml"
    "pages/my_bricks/SectionDelegate.qml"
    "pages/grid/GridPage.qml"
    "pages/grid/RecipesModel.qml"
    "pages/grid_view/GridViewPage.qml"
    "pages/grid_view/RecipesModel.qml"
    "pages/list_view/ListViewPage.qml"
    "pages/list_view/RecipesModel.qml"
    "pages/list_view/K0002Delegate.qml"
    "pages/list_view/T0002Delegate.qml"
    "pages/list_view/BrickDelegate.qml"
    "pages/list_view/M0002/M0002MainPage.qml"


    RESOURCES
    icons/gallery/20x20/back.png
    icons/gallery/20x20/drawer.png
    "icons/gallery/20x20/menu.png"
    "icons/gallery/20x20/show_details.png"
    "icons/gallery/20x20/hide_details.png"
    "icons/gallery/20x20@2/back.png"
    "icons/gallery/20x20@2/drawer.png"
    "icons/gallery/20x20@2/menu.png"
    "icons/gallery/20x20@2/show_details.png"
    "icons/gallery/20x20@2/hide_details.png"
    "icons/gallery/20x20@3/back.png"
    "icons/gallery/20x20@3/drawer.png"
    "icons/gallery/20x20@3/menu.png"
    "icons/gallery/20x20@3/show_details.png"
    "icons/gallery/20x20@3/hide_details.png"
    "icons/gallery/20x20@4/back.png"
    "icons/gallery/20x20@4/drawer.png"
    "icons/gallery/20x20@4/menu.png"
    "icons/gallery/20x20@4/show_details.png"
    "icons/gallery/20x20@3/hide_details.png"
    "icons/gallery/index.theme"
    "images/arrow.png"
    "images/arrow@2x.png"
    "images/arrow@3x.png"
    "images/arrow@4x.png"
    "images/arrows.png"
    "images/arrows@2x.png"
    "images/arrows@3x.png"
    "images/arrows@4x.png"
    "images/baboon.png"
    images/bricks/K0002.png
    images/bricks/K0002_transparent.png
    images/bricks/K0003.png
    images/bricks/K0003_transparent.png
    images/bricks/K0004.png
    images/bricks/K0004_transparent.png
    "images/config.svg"
    "images/close.svg"
    "images/connected_icon.svg"
    "images/disconnected_icon.svg"
    "images/dashboard.svg"
    "images/device_type.svg"
    "images/id.svg"
    "images/ip_address.svg"
    images/logo.svg
    "images/t0002_connected.svg"
    "images/t0002_disconnected.svg"
    "images/tcp_port.svg"
    "images/temp_connected.svg"
    "images/temp_disconnected.svg"
    "images/temperature.svg"
    "images/forward.svg"
    "images/go-forward.svg"
    "images/go-forward2.svg"
    "images/humidity.svg"
    "images/pressure.svg"
    "images/qt-logo.png"
    "images/qt-logo@2x.png"
    "images/qt-logo@3x.png"
    "images/qt-logo@4x.png"
    "images/udp_port.svg"
    images/warning.svg
    images/warning_red.svg
    "images/wifi.svg"
    "scxml/EmulatorM0002States.scxml"
    "scxml/PseudoDNSStates.scxml"
    "qmldir"
    "qtquickcontrols2.conf"
    QML_FILES pages/controls/BrickTile.qml
    QML_FILES CodingConventions.qml
    QML_FILES pages/HomePage.qml
    QML_FILES PagesModel.qml
    SOURCES BrickCommunicationWrapper.hpp
    SOURCES BrickCommunicationWrapper.cpp

)

qt6_add_statecharts(kikoBricks
    "scxml/EmulatorM0002States.scxml"
    "scxml/PseudoDNSStates.scxml"
)

set_property(TARGET kikoBricks APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

target_link_libraries(kikoBricks PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2
    Qt::Quick3D
    Qt::Scxml
    Qt6::Test
)

if(UNIX AND NOT APPLE AND CMAKE_CROSSCOMPILING)
    find_package(Qt6 REQUIRED COMPONENTS QuickTemplates2)

    # Work around QTBUG-86533
    target_link_libraries(kikoBricks PRIVATE Qt::QuickTemplates2)
endif()

install(TARGETS kikoBricks
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
