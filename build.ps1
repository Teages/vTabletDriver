function Build {
    param (
        [parameter(Mandatory=$true)]
        [string] $buildType 
    )
    process {
        $location = Get-Location
        $currentDrive = Split-Path -qualifier $location.Path
        $logicalDisk = Gwmi Win32_LogicalDisk -filter "DriveType = 4 AND DeviceID = '$currentDrive'"
        $cd = $location.Path.Replace($currentDrive, $logicalDisk.ProviderName)
        
        [bool] $release = $($buildType.ToLower() -eq "release") -or $($buildType.ToLower() -eq "r");
        $configName = $(if ($release) {'Release'} else {'Debug'})
        $buildPath = Join-Path -Path $cd -ChildPath "build\$($configName)"

        Write-Host "Build path is '$($buildPath)'" -foregroundcolor white

        if ($release) {
            MSBuild "$($cd)\TabletDriver.sln" 1 1
        }
        else {
            MSBuild "$($cd)\TabletDriver.sln" 1 0
        }

        Write-Host "Copying all files to '$($buildPath)'..."
        & xcopy /C /Y "$($cd)\TabletDriverGUI\bin\netcoreapp3.0\*" "$($buildPath)\*"
        & xcopy /C /Y "$($cd)\TabletDriverService\bin\*" "$($buildPath)\bin\*"
        & xcopy /C /Y "$($cd)\VMulti Installer GUI\bin\*" "$($buildPath)\bin\*"
        
        Write-Host "Compressing files to '$($buildPath)\build.zip'..."
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
        $args = $path
        
        if ($isRelease) {
            $args += " /p:Configuration=Release"
        }
        else {
            $args += " /p:Configuration=Debug"
        }

        if ($publish) {
            $args += " /t:Publish"
        }
        else {
            $args += " /t:Rebuild"
        }

        Write-Host "Build started..." -foregroundcolor green
        & $msb "$($args)"
    }
}

$name = Read-Host -Prompt 'Input configuration type (release/debug)'
Build $name