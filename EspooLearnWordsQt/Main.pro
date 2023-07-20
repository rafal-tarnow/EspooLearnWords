include(./autostart/qautostart.pri)

TEMPLATE = app
TARGET = Aspoo
QT += quick quickcontrols2 sql network websockets mqtt charts printsupport

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

HEADERS += \
    contactmodel.h \
    database.hpp \
    iot/MessagesModel.h \
    iot/customtablemodel.h \
    iot/mqtt/clientsubscription.h \
    iot/mqtt/websocketiodevice.h \
    iot/plot/qcustomplot.h \
    iot/plot/qmlplot.h \
    model/qqmlsortfilterproxymodel.hpp \
#    modules/DevicesManager.h \
#    modules/M0001/M0001.h \
#    modules/M0001/M0001Controller.h \
#    modules/M0002/M0002.h \
#    modules/M0002/M0002Controller.h \
    test_model.hpp \
    test_model_2.hpp

SOURCES += \
    database.cpp \
    iot/MessagesModel.cpp \
    iot/customtablemodel.cpp \
    iot/mqtt/clientsubscription.cpp \
    iot/mqtt/websocketiodevice.cpp \
    iot/plot/qcustomplot.cpp \
    iot/plot/qmlplot.cpp \
    main.cpp \
    contactmodel.cpp \
    model/qqmlsortfilterproxymodel.cpp \
#    modules/DevicesManager.cpp \
#    modules/M0001/M0001.cpp \
#    modules/M0001/M0001Controller.cpp \
#    modules/M0002/M0002.cpp \
#    modules/M0002/M0002Controller.cpp \
    test_model.cpp \
    test_model_2.cpp

RESOURCES += \
    pages/BusyIndicatorPage.qml \
    pages/ButtonPage.qml \
    pages/CheckBoxPage.qml \
    pages/ComboBoxPage.qml \
    pages/DelayButtonPage.qml \
    pages/DelegatePage.qml \
    pages/DevicesPage.qml \
    pages/DialogPage.qml \
    pages/DialPage.qml \
    pages/DictionaryPage.qml \
    pages/FramePage.qml \
    pages/GroupBoxPage.qml \
    pages/M0001ControlPage.qml \
    pages/M0002ControlPage.qml \
    pages/telemetry/MqttMessagesPage.qml \
    pages/telemetry/NewConnectionDialog.qml \
    pages/telemetry/DisconnectDialog.qml \
    pages/PageIndicatorPage.qml \
    pages/ProgressBarPage.qml \
    pages/RadioButtonPage.qml \
    pages/RangeSliderPage.qml \
    pages/ScrollablePage.qml \
    pages/ScrollBarPage.qml \
    pages/ScrollIndicatorPage.qml \
    pages/SliderPage.qml \
    pages/SpinBoxPage.qml \
    pages/StackViewPage.qml \
    pages/SwipeViewPage.qml \
    pages/SwitchPage.qml \
    pages/TabBarPage.qml \
    pages/TextAreaPage.qml \
    pages/TextFieldPage.qml \
    pages/ToolTipPage.qml \
    pages/TumblerPage.qml \
    AddEditDialog.qml \
    ContactDelegate.qml \
    ContactDelegateMy.qml \
    ContactForm.ui.qml \
    ContactView.qml \
    Main.qml \
    MainTest.qml \
    DebugRectangle.qml \
    designer/Backend/ContactModel.qml \
    SectionDelegate.ui.qml \
    SearchBar.qml \
    SectionScrollBar.qml \
    SectionScrollIndicator.qml \
    +Material/ToolBar.qml \
    ToolBar.qml \
    icons/espoo/index.theme \
    icons/espoo/20x20/back.png \
    icons/espoo/20x20/show_details.png \
    icons/espoo/20x20/hide_details.png \
    icons/espoo/20x20/drawer.png \
    icons/espoo/20x20/menu.png \
    icons/espoo/20x20@2/back.png \
    icons/espoo/20x20@2/show_details.png \
    icons/espoo/20x20@2/hide_details.png \
    icons/espoo/20x20@2/drawer.png \
    icons/espoo/20x20@2/menu.png \
    icons/espoo/20x20@3/back.png \
    icons/espoo/20x20@3/show_details.png \
    icons/espoo/20x20@3/hide_details.png \
    icons/espoo/20x20@3/drawer.png \
    icons/espoo/20x20@3/menu.png \
    icons/espoo/20x20@4/back.png \
    icons/espoo/20x20@4/show_details.png \
    icons/espoo/20x20@4/hide_details.png \
    icons/espoo/20x20@4/drawer.png \
    icons/espoo/20x20@4/menu.png \
    images/arrow.png \
    images/arrow@2x.png \
    images/arrow@3x.png \
    images/arrow@4x.png \
    images/arrows.png \
    images/arrows@2x.png \
    images/arrows@3x.png \
    images/arrows@4x.png \
    images/qt-logo.png \
    images/qt-logo@2x.png \
    images/qt-logo@3x.png \
    images/qt-logo@4x.png \
    images/qtlogo.svg \
    qml.qrc


# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = $$PWD/designer

OTHER_FILES += \
    designer/Backend/*.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols2/contactlist
INSTALLS += target

DISTFILES += \
    android-sources/AndroidManifest.xml










