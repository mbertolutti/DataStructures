add_executable(DataStructures main.cpp)
target_compile_features(DataStructures PRIVATE cxx_std_20)
find_package(fmt REQUIRED)
target_link_libraries(DataStructures PRIVATE fmt::fmt)
