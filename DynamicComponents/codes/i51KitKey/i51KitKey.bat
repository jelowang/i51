copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
Semo i51KitKey.se
i51Compiler i51KitKey.se
PackageMaker.exe i51KitKey -re -kit -i51
del *.exe
del *.o
del *.axf


copy .\debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitKey\*.dll
copy .\*.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitKey\*.i51

copy .\debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\PKiGAPI\*.dll
copy .\*.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\PKiGAPI\*.kit

copy .\debug\*.dll ..\PKiGAPI\*.dll
copy .\debug\*.dll .\*.dll
copy .\*.kit ..\PKiGAPI\*.kit