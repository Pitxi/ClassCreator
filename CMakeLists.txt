# ------------------------------------------------------------------------------
# A GTK based C++ class creation tool.
#
# Class Creator works as a wizard, retrieving the needed information from the
# user so it can create the source and header files of the class.
#
# Author: Javier Campón Pichardo
# Date: 25/07/2014
# Copyright 2014 Javier Campón Pichardo
#
# Distributeed under the terms of the GNU General Public License version 3 or
# later.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software. If not, see <http://www.gnu.org/licenses/>.
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
# Doxygen documentation
# ---------------------
set(PROJECT_BRIEF "A GTK based C++ class creation tool.") # Brief description of the project.
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
    set(LOCALE_DIR "/usr/share/locale")

    foreach(po_file IN LISTS PO_FILES)
        get_filename_component(language "${po_file}" NAME_WE)

        add_custom_target("po-${language}" ALL
                          mkdir -p "${PROJECT_BINARY_DIR}/locale/${language}/LC_MESSAGES"
                          COMMAND "${MSGFMT_EXECUTABLE}"
                          "--output=${PROJECT_BINARY_DIR}/locale/${language}/LC_MESSAGES/${PACKAGE}.mo" "${po_file}"
                          WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                          COMMENT "Processing PO file for locale '${language}'..." VERBATIM)
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

# --------------
# Install target
# --------------

# Install and uninstall targets will be available only in RELEASE build type.
if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    set(DESKTOP_FILES_DIR "/usr/share/applications")
    set(GSETTINGS_SCHEMAS_DIR "/usr/share/glib-2.0/schemas")
    set(CMAKE_INSTALL_PREFIX "/usr")

    configure_file("${PROJECT_TEMPLATE_DIR}/desktop.in" "${PROJECT_BINARY_DIR}/${PACKAGE}.desktop" @ONLY)

    install(TARGETS classcreator
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
    install(DIRECTORY "${PROJECT_BINARY_DIR}/locale/" DESTINATION "${LOCALE_DIR}"
            FILES_MATCHING PATTERN "*.mo")
    install(DIRECTORY "${PROJECT_BINARY_DIR}/schemas/" DESTINATION "${GSETTINGS_SCHEMAS_DIR}"
            FILES_MATCHING PATTERN "*.xml")
    install(FILES "${PROJECT_SOURCE_DIR}/resources/icons/scalable/${PACKAGE}.svg" DESTINATION "/usr/share/icons/hicolor/scalable/apps") # Scalable icon
    install(FILES "${PROJECT_SOURCE_DIR}/resources/icons/48x48/${PACKAGE}.png" DESTINATION "/usr/share/icons/hicolor/48x48/apps") # 48x48 icon
    install(FILES "${PROJECT_SOURCE_DIR}/resources/icons/256x256/${PACKAGE}.png" DESTINATION "/usr/share/icons/hicolor/256x256/apps") # 256x256 icon
    install(FILES "${PROJECT_BINARY_DIR}/${PACKAGE}.desktop" DESTINATION "${DESKTOP_FILES_DIR}") # Desktop file for system menus/dash.
    install(CODE "execute_process(COMMAND glib-compile-schemas \"${GSETTINGS_SCHEMAS_DIR}\")") # Compiling gschemas.
    install(CODE "execute_process(COMMAND gtk-update-icon-cache \"/usr/share/icons/hicolor\")") # Recreating the icons cache after instalation.

    message(STATUS "${Green}Use '${Blue}make install${Green}' to install the application.${ColourReset}")

    # ----------------
    # Uninstall target
    # ----------------
    configure_file("${PROJECT_TEMPLATE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target(uninstall
                      COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                      COMMAND glib-compile-schemas "${GSETTINGS_SCHEMAS_DIR}"
                      COMMAND gtk-update-icon-cache "/usr/share/icons/hicolor"
                      COMMENT "Deleting installed files...")

    message(STATUS "${Green}Use '${Blue}make uninstall${Green}' to uninstall the application.${ColourReset}")
endif()
