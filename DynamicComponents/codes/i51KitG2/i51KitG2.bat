copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
Semo i51KitG2.se
i51Compiler i51KitG2.se
PackageMaker.exe i51KitG2 -re -kit -i51


del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2 md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2

copy .\Release\i51KitG2.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2\i51KitG2.dll
copy .\i51KitG2.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2\i51KitG2.i51

copy .\Release\i51KitG2.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\PKiGAPI\i51KitG2.dll
copy .\i51KitG2.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\PKiGAPI\i51KitG2.kit

copy .\Release\i51KitG2.dll ..\PKiGAPI\i51KitG2.dll
copy .\Release\i51KitG2.dll .\i51KitG2.dll
copy .\i51KitG2.kit ..\PKiGAPI\i51KitG2.kit

copy .\Debug\i51KitG2.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2\i51KitG2.dll
copy .\i51KitG2.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitG2\i51KitG2.i51

pause