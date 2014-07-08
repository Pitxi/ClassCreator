cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

project ("Class Creator")

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTKMM REQUIRED "gtkmm-3.0 >= 3.8.1")
pkg_check_modules (GTKSVMM REQUIRED "gtksourceviewmm-3.0 >= 3.2.0")

include_directories (${GTKMM_INCLUDE_DIRS} ${GTKSVMM_INCLUDE_DIRS})
link_directories (${GTKMM_LIBRARY_DIRS} ${GTKSVMM_LIBRARY_DIRS})
add_definitions (${GTKMM_CFLAGS_OTHER} ${GTKSVMM_CFLAGS_OTHER})

# -----------------------
# Opciones del compilador
# -----------------------
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

# ---------------
# Message colours
# ---------------
string (ASCII 27 ESCAPE)
set (COLOUR_RESET "${ESCAPE}[m")
set (GREEN "${ESCAPE}[32m")

# ------------------------------
# Configuración de la aplicación
# ------------------------------
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_MICRO 0)
set (APP_ID "org.kazires.classcreator")
set (APP_PATH "/org/kazires/classcreator/")

configure_file ("${PROJECT_SOURCE_DIR}/${APP_ID}.gschema.xml.in" "${PROJECT_BINARY_DIR}/schemas/${APP_ID}.gschema.xml")
configure_file ("${PROJECT_SOURCE_DIR}/${APP_ID}.gresource.xml.in" "${PROJECT_BINARY_DIR}/resources/${APP_ID}.gresource.xml")

execute_process (COMMAND glib-compile-resources --generate-source "--sourcedir=${PROJECT_SOURCE_DIR}" "${APP_ID}.gresource.xml" "--target=${PROJECT_SOURCE_DIR}/src/resources.c"
                 COMMAND glib-compile-resources --generate-header "--sourcedir=${PROJECT_SOURCE_DIR}" "${APP_ID}.gresource.xml" "--target=${PROJECT_SOURCE_DIR}/src/resources.h"
                 WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources"
                 RESULT_VARIABLE EXIT_CODE
                 ERROR_VARIABLE ERROR_MESSAGE)
if (EXIT_CODE GREATER 0)
    message (FATAL_ERROR "Error ${EXIT_CODE}: ${ERROR_MESSAGE}")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Debug")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (DEBUG_DEFINE "#define DEBUG")
    execute_process (COMMAND glib-compile-schemas "${PROJECT_BINARY_DIR}/schemas"
                     RESULT_VARIABLE EXIT_CODE
                     ERROR_VARIABLE ERROR_MESSAGE)
    if (EXIT_CODE GREATER 0)
        message (FATAL_ERROR "Error ${EXIT_CODE}: ${ERROR_MESSAGE}")
    endif ()
endif ()

configure_file ("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h" @ONLY)

# -------------
# Documentación
# -------------
set (PROJECT_BRIEF "Utilidad para generar nuevas clases de C++.")
find_package (Doxygen)
if (DOXYGEN_FOUND)
    configure_file (${PROJECT_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target (doc
                       ${DOXYGEN_EXECUTABLE} "${PROJECT_BINARY_DIR}/Doxyfile"
                       WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                       COMMENT "Generating documentation with Doxygen" VERBATIM)
    message (STATUS "${GREEN}Use make doc to generate the documentation.${COLOUR_RESET}")
endif ()

set (SOURCES
     src/main.cpp
     src/classcreatorassistant.cpp
     src/preferencesdialog.cpp
     src/datepicker.cpp
     src/inheritancetreeview.cpp
     src/assistantpage.cpp
     src/intropage.cpp
     src/classpage.cpp
     src/documentationpage.cpp
     src/confirmpage.cpp
     src/resources.c)

set (HEADERS
     src/config.h
     src/classcreatorassistant.h
     src/preferencesdialog.h
     src/datepicker.h
     src/inheritancetreeview.h
     src/assistantpage.h
     src/intropage.h
     src/classpage.h
     src/documentationpage.h
     src/confirmpage.h
     src/resources.h)

add_executable (classcreator ${SOURCES} ${HEADERS})
target_link_libraries (classcreator ${GTKMM_LIBRARIES} ${GTKSVMM_LIBRARIES})

# -----------------------
# Objetivo de instalación
# -----------------------
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    execute_process (COMMAND "${PKG_CONFIG_EXECUTABLE}" glib-2.0 --variable=exec_prefix
                     RESULT_VARIABLE EXIT_CODE
                     OUTPUT_VARIABLE GLIB2_PREFIX
                     ERROR_VARIABLE ERROR_MESSAGE
                     OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (EXIT_CODE GREATER 0)
        message (FATAL_ERROR "Error ${EXIT_CODE}: ${ERROR_MESSAGE}")
    endif ()

    set (GSETTINGS_SCHEMA_DIR "${GLIB2_PREFIX}/share/glib-2.0/schemas")

    configure_file ("${PROJECT_SOURCE_DIR}/compile-schemas.cmake.in" "${PROJECT_BINARY_DIR}/compile-schemas.cmake" @ONLY)

    install (TARGETS classcreator
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION lib
             ARCHIVE DESTINATION lib)
    install (FILES "${PROJECT_BINARY_DIR}/schemas/${APP_ID}.gschema.xml" DESTINATION "${GSETTINGS_SCHEMA_DIR}")
    install (SCRIPT "${PROJECT_BINARY_DIR}/compile-schemas.cmake")

    message (STATUS "${GREEN}Use make install to install the application.${COLOUR_RESET}")
endif ()

# --------------------------
# Objetivo de desinstalación
# --------------------------
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target (uninstall COMMAND "${CMAKE_COMMAND}" -Wno-dev -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif ()
