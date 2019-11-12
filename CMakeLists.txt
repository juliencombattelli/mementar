# CMake 3.11 is needed for proper C++17 support and for better IMPORTED targets support
cmake_minimum_required(VERSION 3.11)
project(mementar LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(Threads)
find_package(Filesystem COMPONENTS Final Experimental)

# mementar library

add_library(mementar 
  ${PROJECT_SOURCE_DIR}/src/core/archiving_compressing/archiving/Archive.cpp
  ${PROJECT_SOURCE_DIR}/src/core/archiving_compressing/archiving/Header.cpp
  ${PROJECT_SOURCE_DIR}/src/core/archiving_compressing/binaryManagement/BitFileGenerator.cpp
  ${PROJECT_SOURCE_DIR}/src/core/archiving_compressing/binaryManagement/BitFileGetter.cpp
  ${PROJECT_SOURCE_DIR}/src/core/archiving_compressing/compressing/Huffman.cpp
  ${PROJECT_SOURCE_DIR}/src/core/archiving_compressing/compressing/LzCompress.cpp
  ${PROJECT_SOURCE_DIR}/src/core/archiving_compressing/compressing/LzUncompress.cpp
  ${PROJECT_SOURCE_DIR}/src/core/EpisodicTree/ArchivedLeaf.cpp
  ${PROJECT_SOURCE_DIR}/src/core/EpisodicTree/ArchivedLeafNode.cpp
  ${PROJECT_SOURCE_DIR}/src/core/EpisodicTree/CompressedLeaf.cpp
  ${PROJECT_SOURCE_DIR}/src/core/EpisodicTree/CompressedLeafNode.cpp
  ${PROJECT_SOURCE_DIR}/src/core/EpisodicTree/CompressedLeafNodeSession.cpp
  ${PROJECT_SOURCE_DIR}/src/core/EpisodicTree/CompressedLeafSession.cpp
  ${PROJECT_SOURCE_DIR}/src/core/EpisodicTree/Context.cpp
  #${PROJECT_SOURCE_DIR}/src/core/Occasions/OccasionsManager.cpp
  ${PROJECT_SOURCE_DIR}/src/core/Occasions/Subscription.cpp
)
target_include_directories(mementar 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(mementar
  PUBLIC 
    Threads::Threads
    std::filesystem
    std::filesystem
)

# Tests for mementar library

add_executable(archive
  ${PROJECT_SOURCE_DIR}/test/archiving_compressing/archive.cpp
)
target_link_libraries(archive mementar)

add_executable(compress
  ${PROJECT_SOURCE_DIR}/test/archiving_compressing/compress.cpp
)
target_link_libraries(compress mementar)

add_executable(uncompress
  ${PROJECT_SOURCE_DIR}/test/archiving_compressing/uncompress.cpp
)
target_link_libraries(uncompress mementar)

add_executable(btree_test
  ${PROJECT_SOURCE_DIR}/test/Btree/btree_test.cpp
)
target_link_libraries(btree_test mementar)

add_executable(btree_max_insertion
  ${PROJECT_SOURCE_DIR}/test/Btree/max_insertion.cpp
)
target_link_libraries(btree_max_insertion mementar)

add_executable(archived_tree 
  ${PROJECT_SOURCE_DIR}/test/EpisodicTree/archived_tree.cpp
)
target_link_libraries(archived_tree mementar)

add_executable(episodic_tree 
  ${PROJECT_SOURCE_DIR}/test/EpisodicTree/episodic_tree.cpp
)
target_link_libraries(episodic_tree mementar)

add_executable(eventlink 
  ${PROJECT_SOURCE_DIR}/test/EventLink.cpp
)
target_link_libraries(eventlink mementar)