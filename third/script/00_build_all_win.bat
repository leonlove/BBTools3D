@echo off 
REM 当前bat文件路径
set dir_root=%CD%
echo %dir_root%

REM 三方库的源码包路径
set dir_package=%dir_root%\..\packages

REM 三方库的源码编译路径和编译日志路径
set dir_build=%dir_root%\..\build
set dir_log=%dir_root%\..\build\log

REM 三方库的源码编译完成后安装路径
set dir_install=%dir_root%\..\install

REM 三方库的源码编译完成后集成路径，数据处理工具的依赖路径
set dir_deploy=%dir_root%\..\deploy

REM 设置编译参数
set vs_version="Visual Studio 14 2015"
set cmake_generator="Visual Studio 14 2015 Win64"

set "SevenZipPath=C:\Program Files\7-Zip\7z.exe"
:: 检查7z.exe工具是否存在
if not exist "%SevenZipPath%" (
	echo 7-zip not found at %SevenZipPath%.
	exit /b 1
)

set "CMakeExePath=D:\Program Files\CMake\bin\cmake.exe"
:: 检查7z.exe工具是否存在
if not exist "%CMakeExePath%" (
	echo CMake not found at %CMakeExePath%.
	exit /b 1
)

if not exist %dir_build% (
	echo Creating build directory...
	mkdir %dir_build%
)

if not exist %dir_install% (
	echo Creating build install directory...
	mkdir %dir_install%
)

if not exist %dir_deploy% (
	echo Creating build deploy directory...
	mkdir %dir_deploy%
)

if not exist %dir_log% (
    echo Creating build log directory...
	mkdir %dir_log%
)

REM call %dir_root%\win\build_assimp.bat
REM call %dir_root%\win\build_laslib.bat
REM call %dir_root%\win\build_spdlog.bat

REM 编译GDAL，需要proj和geos库
REM 编译proj，需要sqlite3、tiff、curl、googletest库
REM call %dir_root%\win\build_sqlite.bat
REM call %dir_root%\win\build_curl.bat
REM call %dir_root%\win\build_tiff.bat
REM call %dir_root%\win\build_googletest.bat
REM call %dir_root%\win\build_proj.bat

REM call %dir_root%\win\build_geos.bat
call %dir_root%\win\build_gdal.bat

REM pause>nul 