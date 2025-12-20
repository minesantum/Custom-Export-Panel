cmake_minimum_required(VERSION 3.16)
project(custom-export-panel VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt6 - Usar la instalación local de Qt 6.8.3
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# ------------------------------------------------------------------------------
# LibOBS / Frontend Setup
# ------------------------------------------------------------------------------

set(LOCAL_LIBOBS "${CMAKE_SOURCE_DIR}/libs/obs.lib")
set(LOCAL_FRONTEND "${CMAKE_SOURCE_DIR}/libs/obs-frontend-api.lib")

if(EXISTS "${LOCAL_LIBOBS}")
    message(STATUS "Found local obs.lib: ${LOCAL_LIBOBS}")
    add_library(libobs SHARED IMPORTED)
    set_target_properties(libobs PROPERTIES
        IMPORTED_IMPLIB "${LOCAL_LIBOBS}"
    )
else()
    find_package(libobs REQUIRED)
endif()

if(EXISTS "${LOCAL_FRONTEND}")
    message(STATUS "Found local obs-frontend-api.lib: ${LOCAL_FRONTEND}")
    add_library(obs-frontend-api SHARED IMPORTED)
    set_target_properties(obs-frontend-api PROPERTIES
        IMPORTED_IMPLIB "${LOCAL_FRONTEND}"
    )
endif()

# Include Directories
if(DEFINED OBS_SOURCE_DIR)
    # Detectar automáticamente si es 'UI' o 'frontend'
    if(EXISTS "${OBS_SOURCE_DIR}/UI")
        set(OBS_FRONTEND_DIR "${OBS_SOURCE_DIR}/UI")
        set(OBS_FRONTEND_API_PATH "${OBS_FRONTEND_DIR}/obs-frontend-api")
        message(STATUS "Found OBS frontend dir: UI")
    elseif(EXISTS "${OBS_SOURCE_DIR}/frontend")
        set(OBS_FRONTEND_DIR "${OBS_SOURCE_DIR}/frontend")
        message(STATUS "Found OBS frontend dir: frontend")
        
        # Check for 'api' subdir (newer structure) vs 'obs-frontend-api'
        if(EXISTS "${OBS_FRONTEND_DIR}/api")
            set(OBS_FRONTEND_API_PATH "${OBS_FRONTEND_DIR}/api")
        else()
             set(OBS_FRONTEND_API_PATH "${OBS_FRONTEND_DIR}/obs-frontend-api")
        endif()
    else()
        message(WARNING "Could not find OBS frontend directory (UI or frontend)")
    endif()

    target_include_directories(libobs INTERFACE 
        "${CMAKE_SOURCE_DIR}/include"
        "${OBS_SOURCE_DIR}/libobs"
        "${OBS_FRONTEND_DIR}"
        "${OBS_FRONTEND_API_PATH}"
        "${OBS_SOURCE_DIR}/deps/w32-pthreads"
    )
endif()

# ------------------------------------------------------------------------------
# Plugin Target CON QT UI
# ------------------------------------------------------------------------------

add_library(custom-export-panel MODULE
    src/plugin-main.cpp
    src/custom-export-dock.hpp
    src/custom-export-dock.cpp
)

target_link_libraries(custom-export-panel PRIVATE
    libobs
    obs-frontend-api
    Qt6::Widgets
)

if(DEFINED OBS_SOURCE_DIR)
    target_include_directories(custom-export-panel PRIVATE 
        "${CMAKE_SOURCE_DIR}/include"
        "${OBS_SOURCE_DIR}/libobs"
        "${OBS_FRONTEND_DIR}"
        "${OBS_FRONTEND_API_PATH}"
    )
endif()

if(WIN32)
    set_target_properties(custom-export-panel PROPERTIES PREFIX "")
endif()
