## 当前版本

[littlefs - 2.9.3](https://github.com/littlefs-project/littlefs/releases/tag/v2.9.3)。

## 平台编译

### 1、Ubuntu

环境依赖：

```shell
# 构建组件
sudo apt install cmake
sudo apt install ninja-build

# 依赖组件
sudo apt install libfuse-dev
```

编译和运行

```shell
# 配置项目
cmake -G Ninja -B build -D CMAKE_BUILD_TYPE=Debug

# 编译
cmake --build build

# 运行
./build/mklfs <options>
```

## 测试命令

```shell
# MSYS2
./build/mklfs.exe -c ./ -d ./lfs.bin -s 1024 -b 64
```

## 工具

- 在线浏览 littlefs 镜像文件：[littlefs-disk-img-viewer](https://tniessen.github.io/littlefs-disk-img-viewer/)。
