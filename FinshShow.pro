QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


NCLUDEPATH += \
        $$PWD/database\
        $$PWD/appHome\
        $$PWD/animeGan\
        $$PWD/utils


include($$PWD/appHome/appHome.pri)
include($$PWD/database/database.pri)
include($$PWD/animeGan/animeGan.pri)
include($$PWD/utils/utils.pri)




SOURCES += \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    regist.cpp

HEADERS += \
    loginwindow.h \
    mainwindow.h \
    regist.h

FORMS += \
    loginwindow.ui \
    mainwindow.ui \
    regist.ui

TRANSLATIONS += en_US.ts

INCLUDEPATH += \
    "E:/packge/opencv4-cuda/include" \
    "E:/packge/ncnn-20230517-windows-vs2019-debug/include"\
    "E:/VulkanSDK/1.3.250.1/Include"

LIBS += \
    "E:/VulkanSDK/1.3.250.1/Lib/vulkan-1.lib"\
    "E:/packge/opencv4-cuda/x64/vc16/lib/*.lib" \
    "E:/packge/ncnn-20230517-windows-vs2019-debug/lib/*.lib"


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
