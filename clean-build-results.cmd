@echo off
pushd c:\code\AdventOfCode
del /S *.exe
del /S *.pdb
del /S *.ilk 
del /S *.obj
del /S *.dll
del /S *.deps.json
del /S *.runtimeconfig.json
popd