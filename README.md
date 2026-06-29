# 屏幕批注工具 for linux

基于 Qt 构建的简单屏幕批注工具，使用TraeCN(AI编程工具)，适用于 Linux。

## 功能特性
- 基础的绘图工具
- 最小化到系统托盘图标，可快速唤起批注

## Arch系安装方式
为Arch系Linux编写了PKGBUILD，可从Release获取，然后makepkg -si安装

## 手动编译安装（通用）

编译前需要通过系统包管理器获取依赖：
- Qt6开发包(Debian系包名qt6-base-dev;Redhat系包名qt6-qtbase-devel;Arch系开发包已包含在qt6-base中)
- CMake

下载或克隆仓库后运行
```bash
mkdir -p build && cd build
cmake ..
make
sudo make install
sudo cp ../Desktop/screen-annotator.png /usr/share/icons/hicolor/48x48/apps
sudo cp ../Desktop/screen-annotator.desktop /usr/share/applications
```

## 移除手动安装：
```bash
sudo rm /usr/bin/screen-annotator
sudo rm /usr/share/icons/hicolor/48x48/apps/screen-annotator.png
sudo rm /usr/share/applications/screen-annotator.desktop
```
