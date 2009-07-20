include_directories(${CMAKE_SOURCE_DIR}/common)

set(dvb_engine_SRCS
  dvb.cpp
  ../common/common.cpp
  ../common/parser.cpp
  ../common/request.cpp
)

kde4_add_plugin(plasma_engine_dvb ${dvb_engine_SRCS})
target_link_libraries(plasma_engine_dvb ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_engine_dvb DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-dvb.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
