copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
Semo i51Installer.se
i51Compiler i51Installer.se
del *.exe
del *.o
rem del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\*.dll
if exist .\i51Installer.i51 copy .\Debug\*.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\i51Installer.i51

if not exist D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer md D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer
if exist *.i51 copy *.i51 D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\*.i51
if exist *.kit copy *.kit D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\*.kit
if exist *.re copy *.re D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\*.re
if exist .\Debug\i51Installer.dll copy .\Debug\i51Installer.dll D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51Installer\i51Installer.dll
if exist .\i51Installer.i51 copy .\i51Installer.i51 D:\i51-2.0\51pk_lan_modis\MoDIS\WIN32FS\DRIVE_E\i51\i51KIT\i51KernelInstaller\i51Installer.i51
pause