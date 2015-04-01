copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
i51Compiler Benchmark.se
PackageMaker.exe Benchmark -re -kit -i51
del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Benchmark md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Benchmark
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Benchmark\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Benchmark\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Benchmark\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Benchmark\*.dll

pause