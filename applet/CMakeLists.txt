include_directories(${CMAKE_SOURCE_DIR}/common)

set(dvb_SRCS
    configwidget.cpp
    dvb.cpp
    selectiondialog.cpp
    ../common/common.cpp
    ../common/parser.cpp
    ../common/request.cpp
    )

qt4_add_resources( dvb_SRCS image.qrc )

kde4_add_plugin(plasma_applet_dvb ${dvb_SRCS})
target_link_libraries(plasma_applet_dvb ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_applet_dvb DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dvb-default.desktop DESTINATION ${SERVICES_INSTALL_DIR})
