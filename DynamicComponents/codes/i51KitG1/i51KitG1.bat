copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\armcc.exe *.exe
copy ..\..\i51TOOL\armcpp.exe *.exe
copy ..\..\i51TOOL\armar.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
Semo i51KitG1.se
i51Compiler i51KitG1.se
PackageMaker.exe i51KitG1 -re -kit -i51
del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG1 md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG1
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG1\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG1\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG1\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG1\*.dll

