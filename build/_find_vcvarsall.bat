@if not exist _vcvarsall.bat (
	@echo [_vcvarsall.bat] not exists in this PC, finding in Visual Studio directory.
	@for /f "tokens=* USEBACKQ" %%F in (`where /r "C:\Program Files\Microsoft Visual Studio" vcvarsall.bat`) DO (
		@echo @set VCVARSALL_BAT=%%F > _vcvarsall.bat
	)
) else (
	@echo [_vcvarsall.bat] exists in this PC, skip.
)