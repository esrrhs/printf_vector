# printf_vector

[![License](https://img.shields.io/github/license/esrrhs/printf_vector)](https://github.com/esrrhs/printf_vector)
[![Language](https://img.shields.io/github/languages/top/esrrhs/printf_vector)](https://github.com/esrrhs/printf_vector)
[![Release](https://img.shields.io/github/v/release/esrrhs/printf_vector)](https://github.com/esrrhs/printf_vector/releases)
[![Build Status](https://github.com/esrrhs/printf_vector/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/esrrhs/printf_vector/actions)

[English](README.md) | [中文说明](README_CN.md)

类似 libc `printf` 的 C++ 格式化库，但支持用 vector（或自定义输入）传参。纯头文件，开箱即用。

基于 [eyalroz/printf](https://github.com/eyalroz/printf) 改造，增加 vector 风格参数包支持。

## 特性

* C++ 纯头文件（`include/printf_vector.h`）
* 底层使用 `std::snprintf`
* 独立 `printf_vector` 命名空间
* 通过 `input_interface` 支持 vector / 自定义输入
* 提供 `printfv`、`snprintfv`、`format`

## 环境要求

* C++11 及以上
* CMake 3.12+（用于示例与 CI 构建）

## 构建示例

```bash
./build.sh
# 或者
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## 使用方法

将 `include/printf_vector.h` 拷入工程，或通过 CMake `INTERFACE` 目标 `printf_vector` 引用。

示例见 [main.cpp](./main.cpp)：

```cpp
printf_vector::vector_input input;
input.add(1);
input.add(2.2f);
input.add("333", std::strlen("333"));
input.add((void *) 0x4);
input.add(2);
input.add("55555", std::strlen("55555"));
input.add(5);
input.add(6);
input.add(1.12345);

const char *fmt = "Hello, World! int=%d float=%f string=%s pointer=%p short-string=%.*s width-int=%*d short-float=%.2f\n";
printf_vector::printfv(fmt, &input);
```

典型输出：

```text
Hello, World! int=1 float=2.200000 string=333 pointer=0x4 short-string=55 width-int=    6 short-float=1.12
```

也支持：

```cpp
char buffer[1024];
printf_vector::snprintfv(buffer, sizeof(buffer), fmt, &input);

auto str = printf_vector::format(fmt, &input);
```

## 发布说明

GitHub Release 会监视 `include/printf_vector.h` 中的 `PRINTF_VECTOR_VERSION`。在 `master` 上修改该版本号即可自动打 tag 并发布头文件包。

## 许可证

[MIT](LICENSE)
