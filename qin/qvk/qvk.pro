CONFIG += designer \
    plugin \
    release
TARGET = $$qtLibraryTarget(qvirtualkeyboardplugin)
TEMPLATE = lib
HEADERS = 
SOURCES = 
RESOURCES = 
LIBS += -L.
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
include(qvirtualkeyboard.pri)
