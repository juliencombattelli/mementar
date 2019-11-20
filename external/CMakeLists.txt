if(${CMAKE_VERSION} VERSION_LESS 3.14)
  macro(FetchContent_MakeAvailable)
    foreach(contentName IN ITEMS ${ARGV})
        string(TOLOWER ${contentName} contentNameLower)
        FetchContent_GetProperties(${contentName})
        if(NOT ${contentNameLower}_POPULATED)
          FetchContent_Populate(${contentName})
          # Only try to call add_subdirectory() if the populated content
          # can be treated that way. Protecting the call with the check
          # allows this function to be used for projects that just want
          # to ensure the content exists, such as to provide content at
          # a known location.
          if(EXISTS ${${contentNameLower}_SOURCE_DIR}/CMakeLists.txt)
              add_subdirectory(${${contentNameLower}_SOURCE_DIR}
                               ${${contentNameLower}_BINARY_DIR})
          endif()
        endif()
    endforeach()
  endmacro()
endif()

include(FetchContent)

FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        6.0.0
)

FetchContent_MakeAvailable(fmt)