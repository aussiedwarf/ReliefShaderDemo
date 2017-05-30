TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

APPLICATION =nativeclient
PLATFORM=platform
COMPILER=compiler
ARCHITECTURE=arch
BUILD=build

CONFIG(debug, debug|release) {
  BUILD=debug
}

CONFIG(release, debug|release) {
  BUILD=release
}


win32-g++{
  PLATFORM=windows
  COMPILER=mingw
  ARCHITECTURE=x86

  CONFIG(debug, debug|release) {
    LIBS += -L../ad/master/build/windows/mingw/x86/debug/bin
    PRE_TARGETDEPS += ../ad/master/build/windows/mingw/x86/debug/bin/libad.a
  }

  CONFIG(release, debug|release) {
    LIBS += -L../ad/master/build/windows/mingw/x86/release/bin
    PRE_TARGETDEPS += ../ad/master/build/windows/mingw/x86/release/bin/libad.a
  }

  LIBS += -L../SDL2/2.0.5/i686-w64-mingw32/lib
  LIBS += -L../SDL2/SDL2_ttf/2.0.14/i686-w64-mingw32/lib
  LIBS += -L../SDL2/SDL2_image/2.0.1/i686-w64-mingw32/lib
  LIBS += -L../glew/glew-2.0.0/mingw32/lib
  LIBS += -pthread -lad -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lglew32 -lOpenGL32

  INCLUDEPATH +=  ../sdl2/2.0.5/i686-w64-mingw32/include \
                  ../sdl2/2.0.5/i686-w64-mingw32/include/SDL2 \
                  ../sdl2/SDL2_image/2.0.1/i686-w64-mingw32/include \
                  ../sdl2/SDL2_ttf/2.0.14/i686-w64-mingw32/include \
                  ../glew/glew-2.0.0/include

}

linux-g++-32{
  PLATFORM=linux
  COMPILER=gnu
  ARCHITECTURE=x86
}

linux-g++-64{
  PLATFORM=linux
  COMPILER=gnu
  ARCHITECTURE=x64

  CONFIG(debug, debug|release) {
    LIBS += -L../ad/master/build/linux/gnu/x64/debug/bin
    PRE_TARGETDEPS += ../ad/master/build/linux/gnu/x64/debug/bin/libad.a
  }

  CONFIG(release, debug|release) {
    LIBS += -L../ad/master/build/linux/gnu/x64/release/bin
    PRE_TARGETDEPS += ../ad/master/build/linux/gnu/x64/release/bin/libad.a
  }

  LIBS += -pthread -lGL -lGLEW -lSDL2

  LIBS += -lad
}

macx{
  PLATFORM=mac
  COMPILER=clang
  ARCHITECTURE=x64

  QMAKE_CXXFLAGS += -F/Library/Frameworks


  CONFIG(debug, debug|release) {
    LIBS += -L../ad/master/build/mac/clang/x64/debug/bin
    PRE_TARGETDEPS += ../ad/master/build/mac/clang/x64/debug/bin/libad.a
  }

  CONFIG(release, debug|release) {
    LIBS += -L../ad/master/build/mac/clang/x64/release/bin
    PRE_TARGETDEPS += ../ad/master/build/mac/clang/x64/release/bin/libad.a
  }

  LIBS += -F/Library/Frameworks -framework SDL2 -framework SDL2_ttf -framework SDL2_image -framework OpenGL

  LIBS += -lad

}

emscripten{
  PLATFORM=browser
  COMPILER=emscripten
  ARCHITECTURE=x86
  TARGET=nbody.js

  CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS -= -g
    QMAKE_CXXFLAGS += -g4
    QMAKE_LINK += -g4

    LIBS += -L../../ad/master/build/browser/emscripten/x86/debug/bin
    PRE_TARGETDEPS += ../../ad/master/build/browser/emscripten/x86/debug/bin/ad.a
    QMAKE_POST_LINK += ./deploy_js.py build/nativeclient/browser/emscripten/x86/debug/bin/nbody.js site/nbody_debug.js build/nativeclient/browser/emscripten/x86/debug/bin/nbody.js.map site/nbody.js.map &
  }

  CONFIG(release, debug|release) {
    LIBS += -L../../ad/master/build/browser/emscripten/x86/release/bin
    PRE_TARGETDEPS += ../../ad/master/build/browser/emscripten/x86/release/bin/ad.a
    QMAKE_POST_LINK += ./deploy_js.py build/nativeclient/browser/emscripten/x86/release/bin/nbody.js site/nbody.js &
  }

  QMAKE_PRE_LINK += ./prebuild.py
  QMAKE_POST_LINK += ./postbuild.py

  LIBS += ../../../.emscripten_cache/asmjs/sdl2.bc
  LIBS += ../../../.emscripten_cache/asmjs/sdl2-image.bc
  LIBS += ../../../.emscripten_cache/asmjs/sdl2-ttf.bc
  LIBS += ../../../.emscripten_cache/asmjs/freetype.bc

  LIBS += -lad

  QMAKE_CXXFLAGS += -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s USE_FREETYPE=1
# -s SDL2_IMAGE_FORMATS=[png]

  INCLUDEPATH += ../../../.emscripten_cache/asmjs/ports-builds/sdl2/include
  INCLUDEPATH += ../../../.emscripten_cache/asmjs/ports-builds/sdl2-image
  INCLUDEPATH += ../../../.emscripten_cache/asmjs/ports-builds/sdl2-ttf
  INCLUDEPATH += ../../emsdk_portable/emscripten/incoming/system/include/emscripten

  MAKEFILE =    MakefileEmscripten
}




DESTDIR =     build/$${APPLICATION}/$${PLATFORM}/$${COMPILER}/$${ARCHITECTURE}/$${BUILD}/bin
MOC_DIR =     build/$${APPLICATION}/$${PLATFORM}/$${COMPILER}/$${ARCHITECTURE}/$${BUILD}/moc
OBJECTS_DIR = build/$${APPLICATION}/$${PLATFORM}/$${COMPILER}/$${ARCHITECTURE}/$${BUILD}/obj
#MAKEFILE =    build/$${APPLICATION}/$${PLATFORM}/$${COMPILER}/$${ARCHITECTURE}/$${BUILD}/Makefile

INCLUDEPATH += ../ad/master ../glm/glm src


SOURCES +=  main.cpp 


HEADERS +=  Camera.hpp



