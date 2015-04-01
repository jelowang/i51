copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\packagemaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
Semo i51KitServer.se
i51Compiler i51KitServer.se
packagemaker i51KitServer -kit
del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitServer md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitServer
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitServer\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitServer\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51KitServer\*.re

