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
