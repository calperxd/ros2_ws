# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_esp_node_temp_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED esp_node_temp_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(esp_node_temp_FOUND FALSE)
  elseif(NOT esp_node_temp_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(esp_node_temp_FOUND FALSE)
  endif()
  return()
endif()
set(_esp_node_temp_CONFIG_INCLUDED TRUE)

# output package information
if(NOT esp_node_temp_FIND_QUIETLY)
  message(STATUS "Found esp_node_temp: 0.0.0 (${esp_node_temp_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'esp_node_temp' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT esp_node_temp_DEPRECATED_QUIET)
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(esp_node_temp_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${esp_node_temp_DIR}/${_extra}")
endforeach()
