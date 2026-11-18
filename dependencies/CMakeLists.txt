add_subdirectory(union)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE union)

# Prism (https://github.com/ethindp/prism, MPL-2.0) speaks through the running screen reader or a speech engine.
# It is built from its release source as a static library with the plugin's C runtime, so no extra DLL ships with
# YAGA. Building it with MSVC needs the "C++ ATL" component of Visual Studio.
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(PRISM_IPO OFF CACHE STRING "Interprocedural optimization for Prism" FORCE)

FetchContent_Declare(prism
	URL https://github.com/ethindp/prism/archive/refs/tags/v0.18.3.tar.gz
	URL_HASH SHA256=44d67e8f46fa52f912127eb69ae46298371c7078f1e33d2a5d7001685844ae1b
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(prism)

# CMake knows MSVC as a C23 compiler only from 4.0 on. Prism asks for C23, but its C sources belong to legacy backends
# that are not built, so C17 is enough.
if (MSVC AND CMAKE_VERSION VERSION_LESS 4.0)
	set_property(TARGET prism PROPERTY C_STANDARD 17)
endif()

# Prism exports NOMINMAX to its users, but the Gothic API headers need the min and max macros of Windows.h.
get_target_property(PRISM_INTERFACE_DEFINITIONS prism INTERFACE_COMPILE_DEFINITIONS)
list(REMOVE_ITEM PRISM_INTERFACE_DEFINITIONS NOMINMAX)
set_target_properties(prism PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${PRISM_INTERFACE_DEFINITIONS}")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE prism::prism)

# Recast & Detour (https://github.com/recastnavigation/recastnavigation, zlib licence) build the navigation mesh the
# navigator walks on from the loaded world's geometry. Static libraries; the demo, tests and examples are not built.
# 64-bit polygon references, because a Gothic world needs more tiles and polygons than 32-bit ones can address.
set(RECASTNAVIGATION_DEMO OFF CACHE BOOL "" FORCE)
set(RECASTNAVIGATION_TESTS OFF CACHE BOOL "" FORCE)
set(RECASTNAVIGATION_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RECASTNAVIGATION_DT_POLYREF64 ON CACHE BOOL "" FORCE)
# Its CMakeLists asks for CMake 3.1, which CMake 4 no longer accepts without this.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

FetchContent_Declare(recastnavigation
	URL https://github.com/recastnavigation/recastnavigation/archive/refs/tags/v1.6.0.tar.gz
	URL_HASH SHA256=d48ca0121962fa0639502c0f56c4e3ae72f98e55d88727225444f500775c0074
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(recastnavigation)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE RecastNavigation::Recast RecastNavigation::Detour)
