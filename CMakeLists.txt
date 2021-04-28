# CMakeList.txt: h265bitstreamCMake 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("h265bitstreamCMake")

# 将源代码添加到此项目的可执行文件。
add_executable (h265bitstreamCMake "h265bitstreamCMake.cpp" "h265bitstreamCMake.h" "bs.h" "h265_stream.h" "h265_nal.cpp" "h265_stream.cpp")

# TODO: 如有需要，请添加测试并安装目标。
