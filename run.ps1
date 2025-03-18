$exePath = "build/Debug/gaussian_blur.exe"

if (-Not (Test-Path $exePath)) {
    Write-Host "File $exePath needed. Make build firts."
    exit 1
}

Write-Host "Running..."
Start-Process -FilePath "$exePath" -WorkingDirectory "build/Debug"