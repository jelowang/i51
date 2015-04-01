copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armcc.exe *.exe
copy ..\..\i51TOOL\armcpp.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
i51Compiler LiteBox2dDemo.se
PackageMaker.exe LiteBox2dDemo -re -kit -i51
del *.exe
rem del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Box2dDemo md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Box2dDemo
if exist LiteBox2dDemo.i51 copy LiteBox2dDemo.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Box2dDemo\Box2dDemo.i51
if exist LiteBox2dDemo.kit copy LiteBox2dDemo.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Box2dDemo\Box2dDemo.kit
if exist LiteBox2dDemo.re copy LiteBox2dDemo.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Box2dDemo\Box2dDemo.re
if exist .\Debug\LiteBox2dDemo.dll copy .\Debug\LiteBox2dDemo.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\Box2dDemo\Box2dDemo.dll

pause