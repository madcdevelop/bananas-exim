@echo off

:: ---------------------------------
:: Examples for building the project
:: ---------------------------------
:: @Param1 Config: debug, release
:: @Param2 Target: rebuild, clean, [blank]

if [%1]==[] (call :help && goto :EOF) 
if %1==help (call :help) else (call :build_project %1 %2)
exit /B 0

:help
echo.
echo These are parameters for building
echo Param1 Config: Debug, Release
echo Param2 Target: Rebuild, Clean, [blank]
exit /B 0

:build_project
set config=%~1
set target=%~2

if [%2]==[] (msbuild bananas-exim.sln -p:Configuration=%config% -t:build) else (msbuild bananas-exim.sln -p:Configuration=%config% -t:%target%)

echo.
if [%2]==[] (echo Command Run: msbuild bananas-exim.sln -p:Configuration=%config% -t:build) else (echo Command Run: msbuild bananas-exim.sln -p:Configuration=%config% -t:%target%)
exit /B 0