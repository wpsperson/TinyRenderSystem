rd /s/q Debug
rd /s/q Release
rd /s/q bin
rd /s/q lib
rd /s/q build
rd /s/q CMakeFiles
rd /s/q Win32
rd /s/q ipch
for /d %%i in (*.dir) do (
rd /s/q %%i
)


del *.cmake
del *.sln
del *.sdf
del *.vcxproj
del *.vcxproj.filters
del *.vcxproj.user
del *.suo
del /a:h *.suo
del CMakeCache.txt
del *.v11.suo
del *.v10.suo
