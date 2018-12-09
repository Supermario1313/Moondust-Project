set(QT_EXTRA_LIBS)
set(QT_EXTRA_LIBS_PRE)

set(QT_FOUND_EXTRA_LIBS)
set(QT_FOUND_EXTRA_LIBS_PRE)

# TODO: Make more accurate detection of Static Qt build than
# based on detection of Qt-side built third-part libraries
# are may appear in any Qt build (static or shared)
# if a target platform has no those libraries

# Manually turn on static Qt deployment
if(PGE_ENABLE_STATIC_QT)

    find_library(QT_PRCE2 qtpcre2)
    if(QT_PRCE2)
        # message("==Qt-LibPNG detected! (${QT_PRCE2})==")
        list(APPEND QT_FOUND_EXTRA_LIBS ${QT_PRCE2})
        set(PGE_QT_STATIC_DETECTED TRUE)
    endif()

    find_library(QT_HARFBUZZ qtharfbuzz)
    if(QT_HARFBUZZ)
        # message("==Qt-HarfBuzz detected! (${QT_HARFBUZZ})==")
        list(APPEND QT_FOUND_EXTRA_LIBS ${QT_HARFBUZZ})
        set(PGE_QT_STATIC_DETECTED TRUE)
    endif()

    find_library(QT_FREETYPE qtfreetype)
    if(QT_FREETYPE)
        message("==Qt-FreeType detected! (${QT_FREETYPE})==")
        list(APPEND QT_FOUND_EXTRA_LIBS ${QT_FREETYPE})
    endif()

    find_library(QT_PNG qtlibpng)
    if(QT_PNG)
        # message("==Qt-LibPNG detected! (${QT_PNG})==")
        list(APPEND QT_FOUND_EXTRA_LIBS ${QT_PNG})
    endif()

    find_library(QT_PTHREAD pthread)
    #if(QT_PTHREAD)
    #     message("==pthread detected! (${QT_PTHREAD})==")
    #     set(PGE_QT_STATIC_DETECTED TRUE)
    #endif()

    if(PGE_QT_STATIC_DETECTED)
        message("=== Static Qt build detected! Plugins will be embedded! ===")
        add_definitions("-DQT_STATICPLUGIN")
        set(QT_IMPORT_PLUGINS_MODULE "
            // This file is autogenerated by qmake. It imports static plugin classes for
            // static plugins specified using QTPLUGIN and QT_PLUGIN_CLASS.<plugin> variables.
            #include <QtPlugin>
            "
        )

        if(WIN32)
            set(QT_IMPORT_PLUGINS_MODULE "${QT_IMPORT_PLUGINS_MODULE}
                Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)"
            )
        elseif(APPLE)
            set(QT_IMPORT_PLUGINS_MODULE "${QT_IMPORT_PLUGINS_MODULE}
                Q_IMPORT_PLUGIN(QMacStylePlugin)
                Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)"
            )
        elseif("${CMAKE_SYSTEM}" MATCHES "Linux")
           set(QT_IMPORT_PLUGINS_MODULE "${QT_IMPORT_PLUGINS_MODULE}
               Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
               Q_IMPORT_PLUGIN(QXcbEglIntegrationPlugin)
               Q_IMPORT_PLUGIN(QXcbGlxIntegrationPlugin)"
           )
        endif()

        set(QT_IMPORT_PLUGINS_MODULE "${QT_IMPORT_PLUGINS_MODULE}
            Q_IMPORT_PLUGIN(QGifPlugin)
            Q_IMPORT_PLUGIN(QICNSPlugin)
            Q_IMPORT_PLUGIN(QICOPlugin)"
        )

        if("${CMAKE_SYSTEM}" MATCHES "Linux")
            set(QT_IMPORT_PLUGINS_MODULE "${QT_IMPORT_PLUGINS_MODULE}
                /* Q_IMPORT_PLUGIN(QEglFSEmulatorIntegrationPlugin) */
                Q_IMPORT_PLUGIN(QEglFSX11IntegrationPlugin)"
            )

            find_package(PkgConfig)
            pkg_check_modules(GTK "gtk+-3.0")
            if(GTK_FOUND)
                set(QT_IMPORT_PLUGINS_MODULE "${QT_IMPORT_PLUGINS_MODULE}
                    Q_IMPORT_PLUGIN(QGtk3ThemePlugin)"
                )
            endif()
        endif()

        # message("Plugins:\n\n${QT_IMPORT_PLUGINS_MODULE}\n\n\n")

        find_library(QT_QGIF    qgif PATHS ${CMAKE_PREFIX_PATH}/plugins/imageformats)
        list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_QGIF})
        find_library(QT_ICNS    qicns PATHS ${CMAKE_PREFIX_PATH}/plugins/imageformats)
        list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_ICNS})
        find_library(QT_ICO     qico PATHS ${CMAKE_PREFIX_PATH}/plugins/imageformats)
        list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_ICO})

        if(APPLE)
            find_library(QT_MACSTYLE qmacstyle PATHS ${CMAKE_PREFIX_PATH}/plugins/styles)
            list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_MACSTYLE})
            find_library(QT_COCOA    qcocoa PATHS ${CMAKE_PREFIX_PATH}/plugins/platforms)
            list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_COCOA})

            list(APPEND QT_FOUND_EXTRA_LIBS cups)

            set(MAC_LIBS_TO_FIND
                Qt5AccessibilitySupport
                Qt5ThemeSupport
                Qt5FontDatabaseSupport
                Qt5GraphicsSupport
                Qt5ClipboardSupport
                Qt5PrintSupport
                DiskArbitration
                IOKit
                Foundation
                CoreServices
                AppKit
                ApplicationServices
                Metal
                CoreFoundation
                CoreGraphics
                OpenGL
                AGL
                Carbon
                QuartzCore
                CoreText
                ImageIO
                Cocoa
                SystemConfiguration
                Security
            )

            foreach(LIB ${MAC_LIBS_TO_FIND})
                    find_library(FOUND_LIB_${LIB} ${LIB})
                    list(APPEND QT_FOUND_EXTRA_LIBS ${FOUND_LIB_${LIB}})
                    #message("Lib: ${LIB}")
                    #message("Found Lib: ${FOUND_LIB_${LIB}}")
            endforeach()
        endif()

        if("${CMAKE_SYSTEM}" MATCHES "Linux")

            find_library(QT_QEGLFS  qeglfs PATHS ${CMAKE_PREFIX_PATH}/plugins/platforms)
            list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_QEGLFS})
            find_library(QT_LINUXFB qlinuxfb PATHS ${CMAKE_PREFIX_PATH}/plugins/platforms)
            list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_LINUXFB})

            # find_library(QT_EGLEMU  qeglfs-emu-integration PATHS ${CMAKE_PREFIX_PATH}/plugins/egldeviceintegrations)
            # list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_EGLEMU})
            find_library(QT_EGLX11  qeglfs-x11-integration PATHS ${CMAKE_PREFIX_PATH}/plugins/egldeviceintegrations)
            if(QT_EGLX11)
                list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_EGLX11})
            else()
                message("!!! QT_EGLX11 NOT FOUND!!!")
            endif()

            find_library(QT_QXCB    qxcb PATHS ${CMAKE_PREFIX_PATH}/plugins/platforms)
            if(QT_QXCB)
                list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_QXCB})
            endif()

            # GTK3
            if(GTK_FOUND)
                find_library(QT_GTK3 qgtk3 PATHS ${CMAKE_PREFIX_PATH}/plugins/platformthemes)
                if(QT_GTK3)
                    list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_GTK3})
                    list(APPEND QT_FOUND_EXTRA_LIBS ${GTK_LIBRARIES})
                endif()
            endif()

            find_library(QT_EGLINT  qxcb-egl-integration PATHS ${CMAKE_PREFIX_PATH}/plugins/xcbglintegrations)
            list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_EGLINT})

            find_library(QT_Qt5EglFSDeviceIntegration  Qt5EglFSDeviceIntegration PATHS ${CMAKE_PREFIX_PATH}/plugins/xcbglintegrations)
            list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_Qt5EglFSDeviceIntegration})

            foreach(qqlib
                    Qt5EventDispatcherSupport Qt5ServiceSupport
                    Qt5ThemeSupport Qt5FontDatabaseSupport Qt5PlatformCompositorSupport)
                find_library(QT_${qqlib} ${qqlib})
                if(QT_${qqlib})
                    message("-- Found ${QT_${qqlib}}!")
                    list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_${qqlib}})
                endif()
            endforeach()

            foreach(qqlib
                        Qt5LinuxAccessibilitySupport
                        Qt5AccessibilitySupport
                        Qt5InputSupport
                        Qt5FbSupport Qt5EglSupport Qt5GlxSupport Qt5XcbQpa
                        Qt5EdidSupport Qt5DeviceDiscoverySupport)
                find_library(QT_${qqlib} ${qqlib})
                if(QT_${qqlib})
                    message("-- Found ${QT_${qqlib}}!")
                    list(APPEND QT_FOUND_EXTRA_LIBS ${QT_${qqlib}})
                endif()
            endforeach()

            find_library(QT_GLXINT  qxcb-glx-integration PATHS ${CMAKE_PREFIX_PATH}/plugins/xcbglintegrations)
            list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_GLXINT})

            if(NOT QT_PRCE2)
                find_library(QT_PRCESYS pcre)
                if(QT_PRCESYS)
                    message("== SYS-pcre detected! (${QT_PRCESYS})==")
                    list(APPEND QT_FOUND_EXTRA_LIBS ${QT_PRCESYS})
                endif()
                find_library(QT_PRCE16SYS pcre16)
                if(QT_PRCE16SYS)
                    message("== SYS-pcre16 detected! (${QT_PRCE16SYS})==")
                    list(APPEND QT_FOUND_EXTRA_LIBS ${QT_PRCE16SYS})
                endif()
            endif()

            find_library(QT_GLIB glib NAMES glib glib-2.0)
            if(QT_GLIB)
                # message("==GLib detected! (${QT_GLIB})==")
                list(APPEND QT_FOUND_EXTRA_LIBS ${QT_GLIB})
            endif()

            find_library(QT_XCB xcb-static)
            if(QT_XCB)
                message("==XCB-Static detected! (${QT_XCB})==")
                list(APPEND QT_FOUND_EXTRA_LIBS ${QT_XCB})
#                foreach(xxlib Xext Xrender Xi Xau Xdmcp SM ICE X11)
#                    unset(QT_XXLIB)
#                    find_library(QT_${xxlib} ${xxlib})
#                    if(QT_${xxlib})
#                        message("-- Found ${QT_${xxlib}}!")
#                        list(APPEND QT_FOUND_EXTRA_LIBS ${QT_${xxlib}})
#                    endif()
#                endforeach()
            endif()

            foreach(xxlib
                    Xext
                    xcb-xinerama Xrender xcb-xkb xcb-sync xcb-xfixes xcb-randr
                    xcb-image xcb-shm xcb-keysyms xcb-icccm xcb-shape xcb-glx
                    Xi Xau Xdmcp SM ICE
                    xcb-render-util
                    xcb-render
                    xcb X11 X11-xcb
                    )
                unset(QT_XXLIB)
                find_library(QT_${xxlib} ${xxlib})
                if(QT_${xxlib})
                    message("-- Found ${QT_${xxlib}}!")
                    list(APPEND QT_FOUND_EXTRA_LIBS ${QT_${xxlib}})
                endif()
            endforeach()

            find_library(QT_fontconfig fontconfig)
            list(APPEND QT_FOUND_EXTRA_LIBS ${QT_fontconfig})

            if(NOT QT_FREETYPE)
            #     find_library(QT_freetype freetype)
                list(APPEND QT_FOUND_EXTRA_LIBS freetype$<$<CONFIG:Debug>:d>)
            endif()

            foreach(xxlib Qt5DBus dbus-1)
                unset(QT_XXLIB)
                find_library(QT_${xxlib} ${xxlib})
                if(QT_${xxlib})
                    message("-- Found ${QT_${xxlib}}!")
                    list(APPEND QT_FOUND_EXTRA_LIBS_PRE ${QT_${xxlib}})
                endif()
            endforeach()

            find_library(QT_EGL EGL)
            list(APPEND QT_FOUND_EXTRA_LIBS ${QT_EGL})

            find_library(QT_udev udev)
            list(APPEND QT_FOUND_EXTRA_LIBS ${QT_udev})

            find_library(QT_GL GL)
            list(APPEND QT_FOUND_EXTRA_LIBS ${QT_GL})

            find_library(QT_DL glib NAMES dl)
            if(QT_DL)
                # message("==DL detected! (${QT_DL})==")
                list(APPEND QT_FOUND_EXTRA_LIBS ${QT_DL})
            endif()
        endif()
        # message("==Full list of libs: ${QT_FOUND_EXTRA_LIBS}==")

        set(QT_EXTRA_LIBS ${QT_FOUND_EXTRA_LIBS})
        set(QT_EXTRA_LIBS_PRE ${QT_FOUND_EXTRA_LIBS_PRE})
    endif()

endif() #PGE_ENABLE_STATIC_QT
