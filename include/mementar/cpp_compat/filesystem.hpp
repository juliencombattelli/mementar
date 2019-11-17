#ifndef MEMENTAR_CPP_COMPAT_FILESYSTEM_HPP
#define MEMENTAR_CPP_COMPAT_FILESYSTEM_HPP

// This file allows to detect the presence of <filesystem> or <experimental/filesystem>
// headers, and include the correct.

#if __has_include(<filesystem>)
#include <filesystem>
#define MEMENTAR_HAVE_FILESYSTEM 1
namespace mementar {
namespace filesystem = std::filesystem;
} // namespace mementar
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
#define MEMENTAR_HAVE_FILESYSTEM 1
#define MEMENTAR_EXPERIMENTAL_FILESYSTEM 1
namespace mementar {
namespace filesystem = std::experimental::filesystem;
} // namespace mementar
#else
#define MEMENTAR_HAVE_FILESYSTEM 0
#endif

#endif // MEMENTAR_CPP_COMPAT_FILESYSTEM_HPP