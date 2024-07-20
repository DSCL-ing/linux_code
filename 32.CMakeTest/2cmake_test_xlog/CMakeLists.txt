
cmake_minimum_required(VERSION 3.20)

#可以自动生成,一般写主项目名称,其他是测试
project(xlog)

include_directories(xlog)

add_library(xlog SHARED xlog/xlog.cc)

add_executable(test_xlog test_xlog/test_xlog.cc xlog)

target_link_libraries(test_xlog xlog)
