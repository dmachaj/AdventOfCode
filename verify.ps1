param([string]$SubDir)

$exePath = Join-Path (Get-Location) "main.exe"

if (!(Test-Path $exePath))
{
    Write-Host "C++ exe not found, trying C# exe"
    $exePath = Join-Path (Get-Location) "\bin\Debug\net8.0\main.exe"
}

if (!(Test-Path $exePath))
{
    throw "$exePath not found"
}

# Set an environment variable to the subdirectory name so that the C++ code can decide at runtime whether to run part1 or part2
$env:PART_TO_RUN = $SubDir;

$sampleInputs = Get-ChildItem (".\" + $subdir + "\*.in")
foreach ($input in $sampleInputs)
{
    Write-Host "Verifying" $input.Name
    $answerFile = $input.FullName.Replace(".in", ".ans")
    if (Test-Path $answerFile)
    {
        $stopwatch = [system.diagnostics.stopwatch]::StartNew()
        $output = Invoke-Expression "type $input | $exePath"
        if ($lastexitcode -ne 0)
        {
            Write-Error "Program crashed"
            continue;
        }
        elseif ($null -eq $output)
        {
            Write-Warning "No output from program."
            continue;
        }
        $stopwatch.Stop()
        $runtimeInMS = [math]::Round($stopwatch.Elapsed.TotalMilliseconds)

        $expected = Get-Content $answerFile
        Write-Host "Expected: $expected`r`n  Actual: $output"

        $differences = (Compare-Object -ReferenceObject $output -DifferenceObject $expected -CaseSensitive)
        [bool]$equal =  $null -eq $differences
        if ($equal)
        {
            Write-Host -ForegroundColor Green "Test case $input succeeded"
        }
        else
        {
            [string]$left = $differences[0].InputObject
            [string]$right = $differences[1].InputObject
            Write-Host -ForegroundColor Red "Test case $input failed. '$left' doesn't equal '$right'"
        }

        if ($runtimeInMS -lt 1000)
        {
            Write-Host "Run completed in $runtimeInMs milliseconds."
        }
        else
        {
            Write-Host -ForegroundColor Yellow "Run completed in $runtimeInMs milliseconds.  That is too slow!"
        }
    }
    else
    {
        Write-Error "$answerFile not found."
    }

    Write-Host ""
}
