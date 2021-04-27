# h265bitstream
a parser for H.265 bitstream

本项目参考h264bitstream(https://github.com/aizvorski/h264bitstream)开发了一个H.265的码流解析器。
能够解析H.265码流的VPS、SPS、PPS、slice header。

使用方法：
使用Visual Studio 2019打开h265bitstreamVS.sln直接编译即可，会在h265bitstreamVS\x64\Debug目录下产生h265bitstreamVS.exe。
运行命令：
h265bitstreamVS.exe <H.265 input bitstream>

示例：
h265bitstreamVS.exe h265.bin

用其他版本的VS编译时如果报错“error C2054: 在“inline”之后应输入“(””，那是因为inline是c++的特性，在c语言里使用会发生错误，此时在bs.h里加入一行，
#define inline __inline
即可解决
