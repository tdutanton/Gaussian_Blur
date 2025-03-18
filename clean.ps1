$buildDir = "build"
$tempFiles = @("CMakeCache.txt", "CMakeFiles", "*.user", "*.obj", "*.log")

if (Test-Path $buildDir) {
    Write-Host "Removing $buildDir..."
    Remove-Item -Recurse -Force $buildDir
    Write-Host "Folder $buildDir removed." -ForegroundColor Green
}

foreach ($file in $tempFiles) {
    Get-ChildItem -Path . -Recurse -Include $file | ForEach-Object {
        Write-Host "Removing $_ ..."
        Remove-Item -Recurse -Force $_
    }
}

Write-Host "Clean done." -ForegroundColor Cyan