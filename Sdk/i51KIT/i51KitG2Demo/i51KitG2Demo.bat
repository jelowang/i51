copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
i51Compiler i51KitG2Demo.se
del *.exe
del *.o
del *.axf

copy .\debug\i51KitG2Demo.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2Demo\i51KitG2Demo.dll
copy .\i51KitG2Demo.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2Demo\i51KitG2Demo.i51
copy .\*.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2Demo\*.re

pause