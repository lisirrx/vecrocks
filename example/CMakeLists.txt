message(STATUS ${CMAKE_SOURCE_DIR})


add_executable(example example.cpp)
target_include_directories(example PUBLIC
        ${rocksdb_SOURCE_DIR}/include
        ${faiss_SOURCE_DIR}/include
        ${vecrocks_SOURCE_DIR}/include
        ${fmt_SOURCE_DIR}/include)

target_link_libraries(example vecrocks faiss fmt)
