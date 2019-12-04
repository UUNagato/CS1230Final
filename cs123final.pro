# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl xml
TARGET = final
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14

unix:!macx {
    LIBS += -lGLU
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    ui/mainwindow.cpp \
    main.cpp \
    glew-1.10.0/src/glew.c \
    ui/view.cpp \
    ui/viewformat.cpp \
    shapes/openglshape.cpp \
#    uniforms/uniformvariable.cpp \
#    uniforms/uniformwidget.cpp \
    camera/orbitingcamera.cpp \
#    uniforms/varsfile.cpp \
    lib/resourceloader.cpp \
    gl/datatype/vbo.cpp \
    gl/datatype/vboattribmarker.cpp \
    gl/datatype/vao.cpp \
    gl/shader.cpp \
    shapes/shapebase.cpp \
    shapes/quad.cpp \
    gl/objloader.cpp \
    shapes/objmodel.cpp \
    gl/textures/Texture.cpp \
    gl/textures/Texture2D.cpp

HEADERS += \
    ui/mainwindow.h \
    ui_mainwindow.h \
    glew-1.10.0/include/GL/glew.h \
    ui/view.h \
    ui/viewformat.h \
    shapes/sphere.h \
    shapes/openglshape.h \
#    uniforms/uniformvariable.h \
    lib/common.h \
#    uniforms/uniformwidget.h \
    camera/orbitingcamera.h \
    camera/camera.h \
#    uniforms/varsfile.h \
    shapes/cube.h \
    lib/resourceloader.h \
    gl/datatype/vbo.h \
    gl/datatype/vboattribmarker.h \
    gl/shaders/shaderattriblocations.h \
    gl/datatype/vao.h \
    gl/shader.h \
    shapes/shapebase.h \
    shapes/quad.h \
    gl/objloader.h \
    shapes/objmodel.h \
    gl/textures/Texture.h \
    gl/textures/Texture2D.h

FORMS += ui/mainwindow.ui
INCLUDEPATH += glm ui glew-1.10.0/include
DEPENDPATH += glm ui glew-1.10.0/include

DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
OTHER_FILES += shaders/shader.frag \
    shaders/shader.vert

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

QMAKE_CXXFLAGS += -g

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain

macx {
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    shaders/normals/normals.vert \
    shaders/normals/normals.frag \
    shaders/normals/normals.gsh \
    shaders/normals/normalsArrow.gsh \
    shaders/normals/normalsArrow.frag \
    shaders/normals/normalsArrow.vert
