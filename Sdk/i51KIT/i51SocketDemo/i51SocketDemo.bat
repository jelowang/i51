copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
i51Compiler i51SocketDemo.se
PackageMaker.exe i51SocketDemo -re -kit -i51
del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo\*.dll


if not exist D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo md D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo
if exist *.i51 copy *.i51 D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo\*.i51
if exist *.kit copy *.kit D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo\*.kit
if exist .\Debug\*.dll copy .\Debug\*.dll D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51SocketDemo\*.dll