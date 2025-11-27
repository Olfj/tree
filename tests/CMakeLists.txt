include(FetchContent)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)

set(CATCH2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CATCH2_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CATCH2_BUILD_EXTRA_TESTS OFF CACHE BOOL "" FORCE)
set(CATCH2_BUILD_HEADER_ONLY ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(catch2)

add_executable(test_filetree test_filetree.cpp)
target_link_libraries(test_filetree
    PRIVATE
        filetree_lib
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

catch_discover_tests(test_filetree)

# platform tests
if(WIN32)
    add_executable(test_platform_windows test_platform_windows.cpp)
    target_link_libraries(test_platform_windows
        PRIVATE
            filetree_lib
            Catch2::Catch2WithMain
    )
    catch_discover_tests(test_platform_windows)
endif()

if(UNIX AND NOT WIN32)
    add_executable(test_platform_linux test_platform_linux.cpp)
    target_link_libraries(test_platform_linux
        PRIVATE
            filetree_lib
            Catch2::Catch2WithMain
    )
    catch_discover_tests(test_platform_linux)
endif()