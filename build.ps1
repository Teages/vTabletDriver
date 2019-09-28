function Build {
    param (
        [parameter(Mandatory=$true)]
        [string] $buildType 
    )
    process {
        $cd = Get-Location
        
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
    }
}
function MSBuild {
    param (
        [parameter(Mandatory=$true)]
        [string] $path

        [parameter(Mandatory=$false)]
        [bool] $publish = $false

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