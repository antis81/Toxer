# Fixes Qt5 library dependencies required for a static build.

# QtCore
GET_TARGET_PROPERTY (
    qt_core_linked_libs
    Qt5::Core
    INTERFACE_LINK_LIBRARIES
    )

SET (qt_core_debug
    "${_qt5Core_install_prefix}/lib/libqtpcre.a"
    dl
    )
SET (qt_core_release
    "${_qt5Core_install_prefix}/lib/libqtpcre.a"
    dl
    )

SET (qt_core_debug_gen_expr "$<$<NOT:$<CONFIG:Release>>:${qt_core_debug}>")
SET (qt_core_release_gen_expr "$<$<CONFIG:Release>:${qt_core_release}>")
SET_TARGET_PROPERTIES (
    Qt5::Core
    PROPERTIES
    INTERFACE_LINK_LIBRARIES
    "${qt_core_debug_gen_expr};${qt_core_release_gen_expr};${qt_core_linked_libs}"
    )

# QtNetwork
GET_TARGET_PROPERTY (
    qt_network_linked_libs
    Qt5::Network
    INTERFACE_LINK_LIBRARIES
    )

SET (qt_network_debug
    "${_qt5Network_install_prefix}/lib/libssl.a"
    "${_qt5Network_install_prefix}/lib/libcrypto.a"
    )
SET (qt_network_release
    "${_qt5Network_install_prefix}/lib/libssl.a"
    "${_qt5Network_install_prefix}/lib/libcrypto.a"
    )

SET (qt_network_debug_gen_expr "$<$<NOT:$<CONFIG:Release>>:${qt_network_debug}>")
SET (qt_network_release_gen_expr "$<$<CONFIG:Release>:${qt_network_release}>")
SET_TARGET_PROPERTIES (
    Qt5::Network
    PROPERTIES
    INTERFACE_LINK_LIBRARIES
    "${qt_network_debug_gen_expr};${qt_network_release_gen_expr};${qt_network_linked_libs}"
    )

# QtGui
GET_TARGET_PROPERTY (
    qt_gui_linked_libs
    Qt5::Gui
    INTERFACE_LINK_LIBRARIES
    )

SET (qt_gui_debug
    "${_qt5Quick_install_prefix}/lib/libqtharfbuzzng.a"
    )
SET (qt_gui_release
    "${_qt5Quick_install_prefix}/lib/libqtharfbuzzng.a"
    )

SET (qt_gui_debug_gen_expr "$<$<NOT:$<CONFIG:Release>>:${qt_gui_debug}>")
SET (qt_gui_release_gen_expr "$<$<CONFIG:Release>:${qt_gui_release}>")
SET_TARGET_PROPERTIES (
    Qt5::Gui
    PROPERTIES
    INTERFACE_LINK_LIBRARIES
    "${qt_gui_debug_gen_expr};${qt_gui_release_gen_expr};${qt_gui_linked_libs}"
    )

