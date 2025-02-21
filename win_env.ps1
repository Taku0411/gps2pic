# vcpkg のインストールディレクトリを指定
$VCPKG_ROOT = "C:\Users\takumi\vcpkg"

# CMake のツールチェインファイルのパスを指定
$VCPKG_CMAKE_TOOLCHAIN_FILE = "$VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
[System.Environment]::SetEnvironmentVariable("CMAKE_TOOLCHAIN_FILE", $VCPKG_CMAKE_TOOLCHAIN_FILE, [System.EnvironmentVariableTarget]::Process)

Write-Host "Done"
Write-Host "VCPKG_ROOT: $VCPKG_ROOT"
Write-Host "CMAKE_TOOLCHAIN_FILE: $VCPKG_CMAKE_TOOLCHAIN_FILE"

