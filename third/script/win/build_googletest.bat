@echo off 
echo start build_googletest

REM 获取当前路径
set current_dir=%CD% 

if not exist %dir_build% (
	set	dir_build=%current_dir%\..\build
    echo "set dir_build %dir_build%"
)

set log_file=%dir_log%\build_googletest.log
if not exist %log_file% (
	echo. > %log_file%
)

if not exist %dir_package% (
	set	dir_package=%current_dir%\..\package
    echo "set dir_package %dir_package%" >> %log_file%
	echo "set dir_package %dir_package%"
)

if not exist %dir_install% (
	set	dir_install=%current_dir%\..\package
    echo "set dir_install %dir_install%" >> %log_file%
	echo "set dir_install %dir_install%"
)

:: 获取日期和时间
for /f "tokens=2 delims==" %%a in ('wmic os get localdatetime /value') do set datetime=%%a
set year=%datetime:~0,4%
set month=%datetime:~4,2%
set day=%datetime:~6,2%
set hour=%datetime:~8,2%
set minute=%datetime:~10,2%
set second=%datetime:~12,2%

:: 格式化输出且输出到日志文件中
echo %year%-%month%-%day% %hour%:%minute%:%second%
echo %year%-%month%-%day% %hour%:%minute%:%second% >> %log_file%

set package_name=googletest-release-1.10.0.tar.gz
set package_dir=%dir_build%\googletest

:: 解压源码包
cd %dir_build%
if exist %package_dir% (
	del /s /q %package_dir%\*.*
	rd /s /q %package_dir%
	md %package_dir%
) else (
	md %package_dir%
)

:: 使用7z.exe先解压.gz部分
"%SevenZipPath%" x "%dir_package%\%package_name%" -o"%package_dir%"

:: 检查解压.gz部分是否成功
if %ERRORLEVEL% neq 0 (
	echo Failed to extract .gz part of %package_name%.
	echo Failed to extract .gz part of %package_name%. >> %log_file%
	exit /b %ERRORLEVEL%
)

:: 获取解压后的.tar文件名
for %%F in ("%package_dir%\*.tar") do set "TarFile=%%F"

echo %TarFile%

:: 使用7z.exe解压.tar文件
"%SevenZipPath%" x "%TarFile%" -o"%package_dir%"

:: 检查解压.tar部分是否成功
if %ERRORLEVEL% neq 0 (
	echo Failed to extract .tar part of %TarFile%.
	exit /b %ERRORLEVEL%
)

:: 创建cmake源码编译路径
set package_dir_build=%package_dir%\googletest-release-1.10.0\build
if not exist %package_dir_build% (
	mkdir %package_dir_build%
)
cd %package_dir_build%

"%CMakeExePath%" .. -G %cmake_generator% -DCMAKE_INSTALL_PREFIX=%dir_install%\googletest

if ERRORLEVEL 1 goto ERROREXIT

"%CMakeExePath%" --build . --target INSTALL --config Release

:: 获取日期和时间
for /f "tokens=2 delims==" %%a in ('wmic os get localdatetime /value') do set datetime=%%a
set year=%datetime:~0,4%
set month=%datetime:~4,2%
set day=%datetime:~6,2%
set hour=%datetime:~8,2%
set minute=%datetime:~10,2%
set second=%datetime:~12,2%

:: 格式化输出且输出到日志文件中
echo %year%-%month%-%day% %hour%:%minute%:%second%
echo %year%-%month%-%day% %hour%:%minute%:%second% >> %log_file%

echo "building googletest done"