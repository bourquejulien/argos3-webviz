# Largely taken from
# https://github.com/ilpincy/argos3-kilobot/blob/master/src/cmake/ARGoSBuildChecks.cmake
#
# Find the ARGoS package
#
find_package(ARGoS REQUIRED)
include_directories(${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIR})
link_libraries(${ARGOS_LDFLAGS})
string(REPLACE "/lib/argos3" "" ARGOS_PREFIX "${ARGOS_LIBRARY_DIR}")
set(CMAKE_INSTALL_PREFIX ${ARGOS_PREFIX} CACHE STRING "Install path prefix, prepended onto install directories." FORCE)

#
# Check for Lua 5.3
#
find_package(Lua53)
if(LUA53_FOUND)
  set(ARGOS_WITH_LUA ON)
  include_directories(${LUA_INCLUDE_DIR})
endif(LUA53_FOUND)

#
# Set ARGoS include dir
#
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})

#
# Set ARGoS link dir
#
link_directories(${ARGOS_LIBRARY_DIRS})