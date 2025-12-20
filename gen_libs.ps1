param (
    [string]$dlldir,
    [string]$outdir
)

$ErrorActionPreference = "Continue"

function Generate-Def($dllName, $libName) {
    try {
        $baseName = [System.IO.Path]::GetFileNameWithoutExtension($libName)
        $dllPath = "$dlldir\$dllName"
        $defFile = "$outdir\$baseName.def"
        $exportsFile = "$outdir\exports_$baseName.txt"
        
        $libraryInternalName = [System.IO.Path]::GetFileNameWithoutExtension($dllName)

        if (-not (Test-Path $dllPath)) {
            Write-Warning "DLL not found: $dllPath"
            return
        }

        Write-Host "Parsing $dllName -> $defFile"

        cmd /c "dumpbin /EXPORTS `"$dllPath`" > `"$exportsFile`""

        "LIBRARY $libraryInternalName" | Set-Content $defFile -Encoding ASCII
        "EXPORTS" | Add-Content $defFile -Encoding ASCII

        $content = Get-Content $exportsFile
        foreach ($line in $content) {
            if ($line -match "^\s+\d+\s+[a-fA-F0-9]+\s+[a-fA-F0-9]+\s+(.+)$") {
                $symbol = $matches[1].Trim()
                
                # Cleanup internal names
                if ($symbol -match " = ") {
                    $symbol = $symbol -split " = " | Select-Object -First 1
                }
                
                # Cleanup forwarders
                if ($symbol -match " -> ") {
                    $symbol = $symbol -replace " -> .*", ""
                }

                if ($symbol -ne "" -and 
                    $symbol -notmatch "Summary" -and 
                    $symbol -notmatch "dependency" -and 
                    $symbol -notmatch "\[NONAME\]" -and
                    $symbol -notmatch "Dump of file" -and 
                    $symbol -notmatch "File Type" -and 
                    $symbol -notmatch "Section contains" -and 
                    $symbol -notmatch "Import ") {
                     
                    $suffix = ""
                    if ($symbol -match "staticMetaObject") {
                        $suffix = " DATA"
                    }

                    # Smart Quoting
                    # C++ mangled names (??...) or names with spaces/special chars need quotes.
                    # Standard C names (alphanumeric + underscore) do NOT need quotes.
                    if ($symbol -match "^[A-Za-z0-9_]+$") {
                        $finalLine = "$symbol$suffix"
                    }
                    else {
                        $finalLine = "`"$symbol`"$suffix"
                    }
                     
                    $finalLine | Add-Content $defFile -Encoding ASCII
                }
            }
        }
    }
    catch {
        Write-Host "Error processing $dllName : $_" -ForegroundColor Red
    }
}

Generate-Def "obs.dll" "obs.lib"
Generate-Def "obs-frontend-api.dll" "obs-frontend-api.lib"
Generate-Def "Qt6Core.dll" "Qt6Core.lib"
Generate-Def "Qt6Gui.dll" "Qt6Gui.lib"
Generate-Def "Qt6Widgets.dll" "Qt6Widgets.lib"
