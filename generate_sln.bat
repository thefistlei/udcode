@echo off
setlocal

rem 设置 CMake 的路径 (根据你的实际安装路径修改)
set "CMAKE_PATH=C:\Program Files\CMake\bin"

rem 设置构建目录
set "BUILD_DIR=build"

rem 设置生成器 (Visual Studio 版本)
set "GENERATOR="
for /f "tokens=2 delims=v" %%a in ('cl 2^>nul ^| findstr /i "Visual C\+\+"') do (
    set "MSVC_VERSION=%%a"
    if defined MSVC_VERSION (
        echo Detected MSVC version: %MSVC_VERSION%
        if "%MSVC_VERSION%"=="17" (
            set "GENERATOR=-G Visual Studio 17 2022"
        ) else if "%MSVC_VERSION%"=="16" (
            set "GENERATOR=-G Visual Studio 16 2019"
        ) else if "%MSVC_VERSION%"=="15" (
            set "GENERATOR=-G Visual Studio 15 2017"
        ) else (
            echo Unsupported MSVC version. Using default generator.
        )
    )
)
if not defined GENERATOR (
    echo No MSVC detected or unsupported version. Using default generator.
    rem 如果没有检测到MSVC，可以使用其他生成器，例如 Ninja：
    rem set "GENERATOR=-G Ninja"
)


rem 设置构建类型 (Debug 或 Release)
set "BUILD_TYPE=Release"
if "%1"=="/Debug" (
    set "BUILD_TYPE=Debug"
)

echo Using generator: %GENERATOR%
echo Using build type: %BUILD_TYPE%

rem 创建构建目录 (如果不存在)
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

rem 进入构建目录
pushd "%BUILD_DIR%"

rem 执行 CMake 命令
echo Running CMake...
"%CMAKE_PATH%\cmake.exe" .. %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 goto :cmake_error

rem 构建项目 (可选，可以在 Visual Studio 中手动构建)
echo Building the project...
"%CMAKE_PATH%\cmake.exe" --build . --config %BUILD_TYPE%
if errorlevel 1 goto :build_error

echo Solution file generated successfully in "%BUILD_DIR%".
popd
goto :eof

:cmake_error
echo CMake generation failed!
popd
exit /b 1

:build_error
echo Build failed!
popd
exit /b 1
