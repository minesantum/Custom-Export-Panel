$ErrorActionPreference = "Stop"

# Paths
$baseDownloads = "C:\Users\DonKolia\Downloads"
$obsBinDir = "$baseDownloads\obs-studio-windows-x64-407944a27\obs-studio-32.0.4-407944a27-windows-x64\bin\64bit"
$obsSourceDir = "$baseDownloads\obs-studio"
$qtDir = "$baseDownloads\windows-deps-qt6-2025-08-23-x64\lib\cmake\Qt6"
$qtDepsRoot = "$baseDownloads\windows-deps-qt6-2025-08-23-x64"

# Find MSVC Tools
Write-Host "Searching for MSVC Tools..."
$msvcPath = Get-ChildItem "C:\Program Files\Microsoft Visual Studio\2022" -Recurse -Directory -Filter "Hostx64" | 
            Where-Object { $_.FullName -like "*\VC\Tools\MSVC\*\bin\Hostx64\x64" } | 
            Select-Object -First 1

if (-not $msvcPath) {
    Write-Error "Could not find MSVC 2022 tools (dumpbin.exe, lib.exe). Please install C++ desktop development workload."
}

$binPath = $msvcPath.FullName
$dumpbin = "$binPath\dumpbin.exe"
$libTool = "$binPath\lib.exe"

Write-Host "Found tools at: $binPath"

# Prepare Libs Directory
$libsDir = "$PSScriptRoot\libs"
if (-not (Test-Path $libsDir)) { New-Item -ItemType Directory -Path $libsDir | Out-Null }

# Function to generate .lib
function Generate-Lib($dllName, $libName) {
    $dllPath = "$obsBinDir\$dllName"
    $libOut = "$libsDir\$libName"
    
    if (-not (Test-Path $dllPath)) {
        Write-Error "DLL not found: $dllPath"
    }

    Write-Host "Generating import lib for $dllName..."
    
    # Dump exports
    & $dumpbin /EXPORTS $dllPath > "$libsDir\exports.txt"
    
    # Create .def file
    $defFile = "$libsDir\$libName.def"
    "LIBRARY $libName" | Set-Content $defFile
    "EXPORTS" | Add-Content $defFile
    
    # Parse exports (skip header, get symbol name)
    # Format is usually: ordinal hint RVA      name
    # We regex for the name
    $content = Get-Content "$libsDir\exports.txt"
    foreach ($line in $content) {
        if ($line -match "^\s+\d+\s+[A-F0-9]+\s+[A-F0-9]+\s+(.+)$") {
            $symbol = $matches[1]
            if ($symbol -notmatch "^\[") { # skip special entries
                $symbol | Add-Content $defFile
            }
        }
    }
    
    # Create .lib
    & $libTool /DEF:$defFile /OUT:$libOut /MACHINE:x64
}

Generate-Lib "obs.dll" "obs.lib"
Generate-Lib "obs-frontend-api.dll" "obs-frontend-api.lib"

# Configure CMake
Write-Host "`nConfiguring CMake..."
$buildDir = "$PSScriptRoot\build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }

Set-Location $buildDir

cmake -S .. -B . `
    -DQt6_DIR="$qtDir" `
    -DOBS_SOURCE_DIR="$obsSourceDir" `
    -DCMAKE_PREFIX_PATH="$qtDepsRoot"

if ($LASTEXITCODE -ne 0) { Write-Error "CMake Configuration Failed" }

Write-Host "`nBuilding..."
cmake --build . --config Release

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n[SUCCESS] Plugin compiled!"
    Write-Host "Location: $buildDir\Release\custom-export-panel.dll"
} else {
    Write-Error "Build Failed"
}
