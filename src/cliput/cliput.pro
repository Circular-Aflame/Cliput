QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algor.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    picon.cpp \
    preview/previewscene.cpp \
    preview/previewview.cpp \
    slider.cpp \
    subtitle.cpp \
    timebox.cpp \
    timeline/timelinescene.cpp \
    timeline/timelineview.cpp \
    timeline/timemarker.cpp \
    timeline/timescale.cpp \
    trackitem.cpp \
    widgets/listwidget.cpp \
    widgets/materialwidget.cpp \
    widgets/previewwidget.cpp \
    widgets/sourcewidget.cpp \
    widgets/timelinewidget.cpp

HEADERS += \
    algor.h \
    captiontrack.h \
    mainwindow.h \
    model.h \
    picon.h \
    picturetrack.h \
    preview/previewscene.h \
    preview/previewvideo.h \
    preview/previewview.h \
    processor.h \
    slider.h \
    subtitle.h \
    timebox.h \
    timeline/effectdialog.h \
    timeline/timelinescene.h \
    timeline/timelineview.h \
    timeline/timemarker.h \
    timeline/timescale.h \
    toolaction.h \
    trackitem.h \
    waiting.h \
    widgets/listwidget.h \
    widgets/materialwidget.h \
    widgets/previewwidget.h \
    widgets/sourcewidget.h \
    widgets/timelinewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += "D:\download\ffmpeg-5.1-full_build-shared\include"
LIBS += -LD:\download\ffmpeg-5.1-full_build-shared\lib -lavutil -lavformat -lavcodec -lavfilter -lswscale -lswresample

RESOURCES += \
    pics.qrc

RC_FILE += ICONEXE.rc
