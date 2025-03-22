@echo off
:: 保存当前目录
pushd %CD%

:: 获取所有子模块并更新到最新
echo Initializing and updating git submodules...
git submodule update --progress --init --recursive --force --remote

:: 切换到目标目录
cd ..\HEngine\vendor\msdf-atlas-gen\msdfgen\freetype

:: 切换到指定提交哈希
echo Checking out commit 2d57b0592805c76d676b51fbf9553de71c5a5c78...
git checkout 2d57b0592805c76d676b51fbf9553de71c5a5c78

:: 返回原始目录
popd

echo Done!
pause
