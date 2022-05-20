QT       += core gui opengl openglwidgets 3dcore

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    renderer.cpp \
    widget.cpp

HEADERS += \
    renderer.h \
    widget.h

FORMS += \
    widget.ui


INCLUDEPATH +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\assimp\include"
INCLUDEPATH +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\glew-2.1.0\include"
INCLUDEPATH +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\freeglut-3.2.1\freeglut-3.2.1\include"

LIBS +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\assimp\release\lib\RelWithDebInfo\assimp-vc143-mt.lib"
LIBS +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\glew-2.1.0\rel\lib\Release\glew.lib"
LIBS +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\glew-2.1.0\rel\lib\Release\glew-shared.lib"
LIBS +="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64\OpenGL32.Lib"
LIBS +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\freeglut-3.2.1\freeglut-3.2.1\rel\lib\Release\freeglut.lib"
LIBS +="C:\Users\Tess\Documents\Visual Studio 2019\Projects\freeglut-3.2.1\freeglut-3.2.1\rel\lib\Release\freeglut_static.lib"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
