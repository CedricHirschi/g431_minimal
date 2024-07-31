$start = Get-Date

Write-Host "MLNDEV Flasher v1.0.0" -ForegroundColor Magenta
Write-Host "-----------------------"


if ($args.Length -lt 2) {
    Write-Host "Usage: flash.ps1 <file> <jlink|jlink-rtt|stlink>" -ForegroundColor Yellow
    exit 1
}

# take first argument as file name
$fileName = $args[0]
# take second argument as programming method, if empty, use jlink
$method = $args[1]


# Install powershell-yaml if not installed
if (-not (Get-Module -ListAvailable -Name powershell-yaml)) {
    Write-Host "Installing powershell-yaml module..." -ForegroundColor Yellow
    Install-Module -Name powershell-yaml -Scope CurrentUser -Force -Confirm
    Write-Host "powershell-yaml module installed" -ForegroundColor Green
}


# Get settings from flash-settings.yaml
$settings = Get-Content .vscode\flash-settings.yaml | ConvertFrom-Yaml
Write-Host "Device: " -NoNewline
Write-Host $settings.device -ForegroundColor Blue
Write-Host "Interface: " -NoNewline
Write-Host $settings.interface -ForegroundColor Blue
Write-Host "Speed: " -NoNewline
Write-Host $settings.speed -ForegroundColor Blue


# echo flashing method in blue
Write-Host "Programming method: " -NoNewline
Write-Host $method -ForegroundColor Blue

$fileName = (Get-Item $fileName).DirectoryName + "\" + (Get-Item $fileName).BaseName + ".bin"

# echo file name in blue
Write-Host "Flashing file: " -NoNewline
Write-Host $fileName -ForegroundColor Blue
Write-Host


# if programming method is jlink
if ($method -eq "jlink") {

# Create JLink script
@"
loadfile $fileName 0x08000000
r
g
q
"@ | Out-File flash.jlink -Encoding ASCII

    # Permanently change terminal color to dark gray
    $prevForegroundColor = $Host.UI.RawUI.ForegroundColor
    $Host.UI.RawUI.ForegroundColor = "DarkGray"

    # Run JLink
    & "C:\Program Files\SEGGER\JLink\JLink.exe" -NoGui 1 -ExitOnError 1 -device $settings.device -if $settings.interface -speed $settings.speed -autoconnect 1 -CommanderScript flash.jlink

    # Change terminal color to default
    $Host.UI.RawUI.ForegroundColor = $prevForegroundColor

    # Remove JLink script
    Remove-Item flash.jlink
}
elseif ($method -eq "jlink-rtt") {

    # Replace .bin with .elf
    $fileName = $fileName -replace ".bin", ".elf"

    # Run JLink
    & "C:\Program Files\SEGGER\JLink\JRun.exe" -device $settings.device -if $settings.interface -speed $settings.speed -s --rtt $fileName

    # Remove JLink script
    Remove-Item flash.jlink
}
elseif ($method -eq "stlink") {
    # Run STLink
    & st-flash write $fileName 0x8000000
}
elseif ($method -eq "cmsis-dap") {
    # Run OpenOCD
    $openocdTarget = $settings.openocd
@"
source [find interface/cmsis-dap.cfg]
adapter speed 8000
transport select swd
source [find target/$openocdTarget]
"@ | Out-File flash.cfg -Encoding ASCII

    # Replace backslashes with forward slashes
    $fileName = $fileName -replace "\\", "/"

    # Replace .bin with .elf
    $fileName = $fileName -replace ".bin", ".elf"

    & openocd -f flash.cfg -c "program $fileName verify reset exit"

    # Remove OpenOCD script
    Remove-Item flash.cfg
}
else {
    Write-Host "Unknown programming method: $method" -ForegroundColor Red
    exit 1
}

Write-Host
Write-Host "Done after " -NoNewline
Write-Host ((Get-Date) - $start).TotalSeconds.ToString("N2") -ForegroundColor Green -NoNewline
Write-Host " seconds"