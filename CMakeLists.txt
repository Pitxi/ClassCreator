# ------------------------------------------------------------------------------
# A GTK based GUI Utility to create the header and source files for a C++ Class.
#
# Copyleft 2014 Javier Campón Pichardo
#
# Distributed under the GNU GPL version 3 license.
#
# This software is distributed WITHOUT ANY WARRANTY; See the license for more
# information.
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("Class Creator")

# ---------------------
# Project configuration
# ---------------------

# Include custom modules path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake modules")

# Colours for messages.
find_package(Colours)

# Use DEBUG build type by default.
if(NOT CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake modules")
set(PROJECT_TEMPLATE_DIR "${PROJECT_SOURCE_DIR}/templates")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_MICRO 1)

set(PACKAGE     "classcreator")
set(LOCALE_DIR  "/usr/share/locale")
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    set(LOCALE_DIR "./locale")
endif()
set(APP_ID      "org.kazires.${PACKAGE}")
set(APP_PATH    "/org/kazires/${PACKAGE}/")

configure_file("${PROJECT_TEMPLATE_DIR}/config.h.in"      "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)
configure_file("${PROJECT_TEMPLATE_DIR}/gschema.xml.in"   "${PROJECT_BINARY_DIR}/schemas/${APP_ID}.gschema.xml" @ONLY)
configure_file("${PROJECT_TEMPLATE_DIR}/gresource.xml.in" "${PROJECT_BINARY_DIR}/${APP_ID}.gresource.xml" @ONLY)

# Compiling local schemas.
add_custom_target(gschema ALL
                  COMMAND glib-compile-schemas "${PROJECT_BINARY_DIR}/schemas"
                  COMMENT "Compiling GLib settings schema file..." VERBATIM)

# Embedding resources into the code.
execute_process(COMMAND glib-compile-resources --generate-source "--sourcedir=${PROJECT_SOURCE_DIR}" "${APP_ID}.gresource.xml" "--target=${PROJECT_SOURCE_DIR}/src/resources.c"
                COMMAND glib-compile-resources --generate-header "--sourcedir=${PROJECT_SOURCE_DIR}" "${APP_ID}.gresource.xml" "--target=${PROJECT_SOURCE_DIR}/src/resources.h"
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                RESULT_VARIABLE   Result
                ERROR_VARIABLE    ErrorOutput)
if(Resutl EQUAL 0)
    message(FATAL_ERROR "${Red}Error Code ${Result}\nError Message: ${ErrorOutput}${ColourReset}")
endif()

# ----------------------
# Compiler configuration
# ----------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# -----------------------
# Libraries configuration
# -----------------------
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED "gtkmm-3.0 >= 3.8.1")
pkg_check_modules(GTKSOURCEVIEWMM REQUIRED "gtksourceviewmm-3.0 >= 3.2.0")

include_directories(${GTKMM_INCLUDE_DIRS} ${GTKSOURCEVIEWMM_INCLUDE_DIRS})
link_directories   (${GTKMM_LIBRARY_DIRS} ${GTKSOURCEVIEWMM_LIBRARY_DIRS})
add_definitions    (${GTKMM_CFLAGS_OTHER} ${GTKSOURCEVIEWMM_CFLAGS_OTHER})

## -----------
# Source code
# -----------
set(SOURCES
    src/resources.c
    src/main.cpp
    src/resourcemanager.cpp
    src/classcreatorassistant.cpp
    src/preferencesdialog.cpp
    src/assistantpage.cpp
    src/intropage.cpp
    src/classinfopage.cpp
    src/docinfopage.cpp
    src/confirmpage.cpp)

set(HEADERS
    src/config.hpp
    src/resources.h
    src/resourcemanager.hpp
    src/classcreatorassistant.hpp
    src/preferencesdialog.hpp
    src/assistantpage.hpp
    src/intropage.hpp
    src/classinfopage.hpp
    src/docinfopage.hpp
    src/confirmpage.hpp)

set(GLADE_FILES
    resources/gui/classinfogrid.glade
    resources/gui/confirmgrid.glade
    resources/gui/docinfogrid.glade
    resources/gui/preferencesgrid.glade)

add_executable("${PACKAGE}" ${SOURCES} ${HEADERS})
target_link_libraries("${PACKAGE}" ${GTKMM_LIBRARIES} ${GTKSOURCEVIEWMM_LIBRARIES})

# ---------------------
# Doxigen documentation
# ---------------------
set(PROJECT_BRIEF "GTK based DOSBox GUI frontend.") # Brief description of the project.
set(DOCUMENTATION_LANGUAGE English) # Documentation language.

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file (${PROJECT_TEMPLATE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)

    # Use 'make doc' to generate the documentation.
    add_custom_target(doc
                      ${DOXYGEN_EXECUTABLE} Doxyfile
                      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                      COMMENT "Generating documentation with Doxygen" VERBATIM)

    message(STATUS "${Green}Use '${Blue}make doc${Green}' to generate the documentation.${ColourReset}")
endif ()

# ------------
# Translations
# ------------
# - Creates a custom target named pot to create the PO Template file.
# - Creates custom targets for every existing translation. This targets will be
#   built within the ALL target.
#   The PO files with the translations must be at translations directory on the
#   project source directory.
#   The MO files will be created at locale directory in the project binary
#   directory.
#   If BUILD_TYPE value is RELEASE the mo files will be installed on install
#   target.
# -----------------------------------------------------------------------------

find_package(XGettext REQUIRED)

set(TRANSLATIONS_DIR "${PROJECT_SOURCE_DIR}/translations")

if(MSGFMT_FOUND)
    file(GLOB PO_FILES "${TRANSLATIONS_DIR}/*.po")

    foreach(po_file IN LISTS PO_FILES)
        get_filename_component(language "${po_file}" NAME_WE)

        add_custom_target("po-${language}" ALL
                          mkdir -p "${PROJECT_BINARY_DIR}/locale/${language}/LC_MESSAGES"
                          COMMAND "${MSGFMT_EXECUTABLE}"
                          "--output=${PROJECT_BINARY_DIR}/locale/${language}/LC_MESSAGES/${PACKAGE}.mo" "${po_file}"
                          WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                          COMMENT "Processint PO file for locale '${language}'..." VERBATIM)
    endforeach()
endif()

if(XGETTEXT_FOUND)
    set(COPYRIGHT_HOLDER "Javier Campón Pichardo")
    set(PACKAGE_BUG_REPORT javiercampon@gmail.com)

    add_custom_target(pot
                      mkdir -p "${TRANSLATIONS_DIR}"
                      COMMAND "${XGETTEXT_EXECUTABLE}"
                      "--default-domain=${PACKAGE}" "--output-dir=${TRANSLATIONS_DIR}" "--output=${PACKAGE}.pot"
                      --from-code=UTF-8 --language=C++ --keyword=_ --indent --sort-by-file
                      "--package-name=${PACKAGE}"
                      "--package-version=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}"
                      "--copyright-holder=${COPYRIGHT_HOLDER}" "--msgid-bugs-address=${PACKAGE_BUG_REPORT}"
                      ${SOURCES} ${HEADERS}
                      COMMAND "${XGETTEXT_EXECUTABLE}"
                      "--default-domain=${PACKAGE}" "--output-dir=${TRANSLATIONS_DIR}" "--output=${PACKAGE}.pot"
                      --from-code=UTF-8 --language=Glade --keyword=_ --indent --sort-by-file --join-existing
                      "--package-name=${PACKAGE}"
                      "--package-version=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}"
                      "--copyright-holder=${COPYRIGHT_HOLDER}" "--msgid-bugs-address=${PACKAGE_BUG_REPORT}"
                      ${GLADE_FILES}
                      WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                      COMMENT "Creating POT file..." VERBATIM)

    message(STATUS "${Green}Use '${Blue}make pot${Green}' to create the PO Template file.${ColourReset}")
    message(STATUS "${Green}The absolute path of the POT file is '${Blue}${TRANSLATIONS_DIR}/${PACKAGE}.pot${Green}'.${ColourReset}")
endif()
