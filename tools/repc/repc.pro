option(host_build)
CONFIG += force_bootstrap
include(3rdparty/moc/moc.pri)

force_bootstrap:isEqual(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 5): error("compiling repc for bootstrap requires Qt 5.5 or higher, due to missing libraries.")

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII QT_NO_CAST_FROM_BYTEARRAY QT_NO_URL_CAST_FROM_STRING
DEFINES += RO_INSTALL_HEADERS=\\\"$$clean_path($$[QT_INSTALL_HEADERS])/QtRemoteObjects\\\"
win32-msvc*:!wince: QMAKE_CXXFLAGS += /wd4129

CONFIG += qlalr
QLALRSOURCES += $$top_srcdir/src/repparser/parser.g
INCLUDEPATH += $$top_srcdir/src/repparser

# fix includes when Qt5 was installed as framework (see qt_functions.prf, keyword FRAMEWORK_INCLUDE)
osx:contains(QT_CONFIG, qt_framework) {
    QMAKE_FRAMEWORKPATH += $$QT.core.libs
    INCLUDEPATH += $$QT.core.libs/QtCore.framework/Headers \
        $$QT.core.libs/QtCore.framework/Headers/$$QT.core.VERSION \
        $$QT.core.libs/QtCore.framework/Headers/$$QT.core.VERSION/QtCore
}

SOURCES += \
    main.cpp \
    repcodegenerator.cpp \
    cppcodegenerator.cpp \
    utils.cpp

HEADERS += \
    repcodegenerator.h \
    cppcodegenerator.h \
    utils.h

load(qt_tool)
