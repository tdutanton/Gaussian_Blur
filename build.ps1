$buildDir = "build"
if (Test-Path $buildDir) {
    Write-Host "Deleting build folder"
    Remove-Item -Recurse -Force $buildDir
}
else {
    Write-Host "Build folder needed. Making folder..."
}

New-Item -ItemType Directory -Path $buildDir
Set-Location $buildDir

Write-Host "CMake generating..."
cmake ..

Write-Host "Building..."
cmake --build . --config Debug
Set-Location ..

Write-Host "Builded!"