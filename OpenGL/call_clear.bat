@echo off
set FolderName=%cd%
for /d %%i in (*) do ( 
	echo "%%i"
	cd %FolderName%\%%i
	call %FolderName%\%%i\clear.bat

)  

pause

