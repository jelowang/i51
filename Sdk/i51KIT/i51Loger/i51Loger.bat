copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
i51Compiler i51Loger.se
PackageMaker.exe i51Loger -re -kit -i51
del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Loger md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Loger
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Loger\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Loger\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Loger\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Loger\*.dll
