@echo off
setlocal enabledelayedexpansion
for /F "usebackq delims=" %%A in (`git rev-parse --show-toplevel`) do set "REPO_ROOT=%%A"
java -jar "%REPO_ROOT%\gradle\wrapper\gradle-wrapper.jar" %*
endlocal
