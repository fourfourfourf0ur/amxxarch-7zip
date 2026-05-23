cmake_minimum_required(VERSION 3.18)

cmake_policy(SET CMP0091 NEW)

project(amxxarch LANGUAGES C CXX)
set(CMAKE_SUPPRESS_REGENERATION ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SRC_DIR "${ROOT_DIR}/src")

set(THIRD_PARTY_DIR "${ROOT_DIR}/third-party")
set(AMXX_DIR "${THIRD_PARTY_DIR}/amxx")
set(HLSDK_DIR "${THIRD_PARTY_DIR}/hlsdk")
set(METAMOD_DIR "${THIRD_PARTY_DIR}/metamod")
set(ZIP_DIR "${THIRD_PARTY_DIR}/7zip")

if(NOT EXISTS "${AMXX_DIR}/sdk/amxxmodule.cpp")
    message(FATAL_ERROR "AMX Mod X SDK was not found: ${AMXX_DIR}/sdk/amxxmodule.cpp")
endif()

if(NOT EXISTS "${ZIP_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "7-Zip backend was not found: ${ZIP_DIR}/CMakeLists.txt")
endif()

add_subdirectory("${ZIP_DIR}")
if(MSVC AND TARGET 7zip)
    set_target_properties(7zip PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

add_library(amxxarch MODULE
    "${AMXX_DIR}/sdk/amxxmodule.cpp"

    "${SRC_DIR}/module.cpp"

    "${SRC_DIR}/amxx/pawn_strings.cpp"

    "${SRC_DIR}/core/archive_format.cpp"
    "${SRC_DIR}/core/job_manager.cpp"
    "${SRC_DIR}/core/logger.cpp"

    "${SRC_DIR}/backend/7z_backend.cpp"
)

if(MSVC)
    set_target_properties(amxxarch PROPERTIES
         MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
     )
endif()

add_library(amxxarch::module ALIAS amxxarch)

target_include_directories(amxxarch PRIVATE
    "${SRC_DIR}"
    "${SRC_DIR}/amxx"

    "${ZIP_DIR}"

    "${AMXX_DIR}"
    "${AMXX_DIR}/sdk"

    "${HLSDK_DIR}"
    "${HLSDK_DIR}/common"
    "${HLSDK_DIR}/dlls"
    "${HLSDK_DIR}/engine"
    "${HLSDK_DIR}/pm_shared"
    "${HLSDK_DIR}/public"

    "${METAMOD_DIR}"
    "${METAMOD_DIR}/common"
    "${METAMOD_DIR}/dlls"
    "${METAMOD_DIR}/engine"
    "${METAMOD_DIR}/pm_shared"
    "${METAMOD_DIR}/public"
)

target_compile_definitions(amxxarch PRIVATE
    AMXXARCH_BUILDING_MODULE=1
    HAVE_STDINT_H=1
    NO_MSVC8_AUTO_COMPAT=1
)

target_link_libraries(amxxarch PRIVATE amxxarch::7zip)

set_target_properties(amxxarch PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
)

if(MSVC)
    target_compile_definitions(amxxarch PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )

    target_compile_options(amxxarch PRIVATE
        /FIwindows_compat.h
        /FIamxx_compat.h
        /EHsc
        /GR
        /permissive-
    )
else()
    target_compile_options(amxxarch PRIVATE
        -include "${SRC_DIR}/amxx/amxx_compat.h"
    )
endif()

if(UNIX AND NOT APPLE)
    target_compile_definitions(amxxarch PRIVATE
        _vsnprintf=vsnprintf
    )

    target_compile_options(amxxarch PRIVATE
        -m32
        -fvisibility=hidden
        -ffunction-sections
        -fdata-sections
    )

    target_link_options(amxxarch PRIVATE
        -m32
        -static-libstdc++
        -static-libgcc
        -Wl,--gc-sections
        -Wl,--strip-all
    )

    if(TARGET 7zip)
        target_compile_options(7zip PRIVATE
            -m32
            -fvisibility=hidden
            -ffunction-sections
            -fdata-sections
        )

        target_link_options(7zip PRIVATE
            -m32
        )
    endif()

    set_target_properties(amxxarch PROPERTIES
        OUTPUT_NAME "amxxarch_amxx_i386"
        SUFFIX ".so"
    )
elseif(WIN32)
    target_compile_definitions(amxxarch PRIVATE
        AMXXARCH_TARGET_WINDOWS=1
    )

    target_link_libraries(amxxarch PRIVATE
        ole32
        oleaut32
    )

    set_target_properties(amxxarch PROPERTIES
        OUTPUT_NAME "amxxarch_amxx"
        SUFFIX ".dll"
    )
endif()