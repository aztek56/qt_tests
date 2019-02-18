# qt_tests
Base project ready to run Qt + OpenGL in CLion

Setup CLion env
===============
- Install MSYS2 (http://www.msys2.org/)
- Run it and do
    pacman -Sy
    pacman —needed -S bash pacman pacman-mirrors msys2-runtime
- Exit and restart console
    pacman -Su
- Exit and restart console
    pacman -S base-devel git mercurial cvs wget p7zip
    pacman -S perl ruby python2 mingw-w64-i686-toolchain mingw-w64-x86_64-toolchain
- Exit and restart console
    pacman -Su
- Exit and restart console
    pacman -S mingw-w64-i686-qt-creator mingw-w64-x86_64-qt-creator
    pacman -S mingw-w64-i686-qt5-static mingw-w64-x86_64-qt5-static
- Point the MinGW env to this installation

