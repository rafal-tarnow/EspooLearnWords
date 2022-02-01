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




