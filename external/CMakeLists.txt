include(FetchContent)

# Fetch fmt git repo and place it into ${SOURCE_DIR}
FetchContent_Populate(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        6.0.0
    SOURCE_DIR     ${PROJECT_SOURCE_DIR}/external/fmt
)

# Add fmt CMake project
add_subdirectory(${PROJECT_SOURCE_DIR}/external/fmt)