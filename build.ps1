function Build {
    param (
        [parameter(Mandatory=$true)]
        [string] $buildType 
    )
    process {
        $location = Get-Location
        $currentDrive = Split-Path -qualifier $location.Path
        $cd = $location.Path.Replace($currentDrive, '')
        
        [bool] $release = $($buildType.ToLower() -eq "release") -or $($buildType.ToLower() -eq "r");
        $configName = $(if ($release) {'Release'} else {'Debug'})
        $buildPath = Join-Path -Path $cd -ChildPath "build\$($configName)"

        Write-Host "Build path is '$($buildPath)'" -foregroundcolor green

        if ($release) {
            & MSBuild "$($cd)\TabletDriver.sln" 1 1
        }
        else {
            & MSBuild "$($cd)\TabletDriver.sln" 1 0
        }

        Write-Host "Wiping '$($buildPath)'..." -foregroundcolor green
        Remove-Item -Path "$buildPath\*"
        
        Write-Host "Copying all files to '$($buildPath)'..." -foregroundcolor green
        Copy-Item -Path "$($cd)\TabletDriverGUI\bin\netcoreapp3.0\*" -Destination "$($buildPath)\" -Recurse
        Copy-Item -Path "$($cd)\TabletDriverService\bin\*" -Destination "$($buildPath)\bin\" -Recurse
        Copy-Item -Path "$($cd)\VMulti Installer GUI\bin\*" -Destination "$($buildPath)\bin\" -Recurse
        
        Write-Host "Compressing files to '$($buildPath)\build.zip'..." -foregroundcolor green
        & "$($Env:ProgramW6432)\7-Zip\7z.exe" a -tzip "$($buildPath)\build.zip" "$($buildPath)\*"
    }
}
function MSBuild {
    param (
        [parameter(Mandatory=$true)]
        [string] $path,

        [parameter(Mandatory=$false)]
        [bool] $publish = $false,

        [parameter(Mandatory=$false)]
        [bool] $isRelease = $false
    )
    process {
        $msb = 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe'
        $args = "$($path)"

        Write-Host "Solution path: $($path)" -foregroundcolor green
        
        if ($publish) {
            $args += " /t:Publish"
        }
        else {
            $args += " /t:Rebuild"
        }

        if ($isRelease) {
            $args += " /p:Configuration=Release"
        }
        else {
            $args += " /p:Configuration=Debug"
        }

        Write-Host "Build started..." -foregroundcolor green
        & $msb "$($args)"
    }
}

$name = Read-Host -Prompt 'Input configuration type (release/debug)'
Build $name
Read-Host