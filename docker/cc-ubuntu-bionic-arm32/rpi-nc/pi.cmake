set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
#set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(tools /opt/cc-ubuntu-bionic-arm32/native-pi-gcc-9.4.0-2)
set(rootfs_dir /opt/cc-ubuntu-bionic-arm32/sysroot)

set(CMAKE_FIND_ROOT_PATH ${rootfs_dir})
set(CMAKE_SYSROOT ${rootfs_dir})

set(CMAKE_LIBRARY_ARCHITECTURE arm-linux-gnueabihf)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE} -L${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -v -fPIC -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE} -L${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -fPIC -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE} -L${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}")

## Compiler Binary 
SET(BIN_PREFIX ${tools}/bin/arm-linux-gnueabihf)

SET (CMAKE_C_COMPILER ${BIN_PREFIX}-gcc-9.4.0)
SET (CMAKE_CXX_COMPILER ${BIN_PREFIX}-g++-9.4.0 )
SET (CMAKE_LINKER ${BIN_PREFIX}-ld 
            CACHE STRING "Set the cross-compiler tool LD" FORCE)
SET (CMAKE_AR ${BIN_PREFIX}-gcc-ar-9.4.0 
            CACHE STRING "Set the cross-compiler tool AR" FORCE)
SET (CMAKE_NM {BIN_PREFIX}-nm 
            CACHE STRING "Set the cross-compiler tool NM" FORCE)
SET (CMAKE_OBJCOPY ${BIN_PREFIX}-objcopy 
            CACHE STRING "Set the cross-compiler tool OBJCOPY" FORCE)
SET (CMAKE_OBJDUMP ${BIN_PREFIX}-objdump 
            CACHE STRING "Set the cross-compiler tool OBJDUMP" FORCE)
SET (CMAKE_RANLIB ${BIN_PREFIX}-ranlib 
            CACHE STRING "Set the cross-compiler tool RANLIB" FORCE)
SET (CMAKE_STRIP {BIN_PREFIX}-strip 
            CACHE STRING "Set the cross-compiler tool RANLIB" FORCE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)