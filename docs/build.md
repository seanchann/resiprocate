# 编译

resiprocate目前支持以cmake的方式的进行编译，推荐使用cmake的方式编译

安装依赖

- sudo yum install popt-devel

编译

- `cd resiprocate`
- `mkdir build.release`
- `cd build.release`
- `cmake .. -DCMAKE_BUILD_TYPE=Release`
- `make`