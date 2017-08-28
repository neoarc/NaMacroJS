@echo off
echo [Batch] Checking remote nalib...
git remote get-url nalib > NUL | find /I /N "fatal: No such remote" > NUL
if "%ERRORLEVEL%" == "0" (
	echo [Batch] Adding remote nalib...
	git remote add nalib https://github.com/neoarc/NaLib.git
)
echo [Batch] Pull subtree ...
git subtree pull -P src/NaLib nalib master
echo [Batch] Done!
@pause