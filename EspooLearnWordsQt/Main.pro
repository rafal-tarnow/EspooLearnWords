include(./autostart/qautostart.pri)

TEMPLATE = app
TARGET = contactlist
QT += quick quickcontrols2 sql


HEADERS += \
    contactmodel.h \
    database.hpp \
    model/qqmlsortfilterproxymodel.hpp \
    test_model.hpp \
    test_model_2.hpp

SOURCES += \
    database.cpp \
    main.cpp \
    contactmodel.cpp \
    model/qqmlsortfilterproxymodel.cpp \
    test_model.cpp \
    test_model_2.cpp

RESOURCES += \
    pages/BusyIndicatorPage.qml \
    pages/ButtonPage.qml \
    pages/CheckBoxPage.qml \
    pages/ComboBoxPage.qml \
    pages/DelayButtonPage.qml \
    pages/DelegatePage.qml \
    pages/DialogPage.qml \
    pages/DialPage.qml \
    pages/DictionaryPage.qml \
    pages/FramePage.qml \
    pages/GroupBoxPage.qml \
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
    icons/espoo/index.theme \
    icons/espoo/20x20/back.png \
    icons/espoo/20x20/drawer.png \
    icons/espoo/20x20/menu.png \
    icons/espoo/20x20@2/back.png \
    icons/espoo/20x20@2/drawer.png \
    icons/espoo/20x20@2/menu.png \
    icons/espoo/20x20@3/back.png \
    icons/espoo/20x20@3/drawer.png \
    icons/espoo/20x20@3/menu.png \
    icons/espoo/20x20@4/back.png \
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
    qml.qrc


# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = $$PWD/designer

OTHER_FILES += \
    designer/Backend/*.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols2/contactlist
INSTALLS += target

DISTFILES += \
    ContactDelegateMy.qml \
    ToolBar.qml




