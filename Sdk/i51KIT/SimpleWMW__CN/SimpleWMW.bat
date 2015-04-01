copy ..\..\i51TOOL\rvtcc.exe *.exe
copy ..\..\i51TOOL\rvarmlink.exe *.exe
copy ..\..\i51TOOL\rvarmcc.exe *.exe
copy ..\..\i51TOOL\rvarmcpp.exe *.exe
copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\armcc.exe *.exe
copy ..\..\i51TOOL\armcpp.exe *.exe
copy ..\..\i51TOOL\armar.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
Semo SimpleWMW.se
i51Compiler SimpleWMW.se
PackageMaker.exe SimpleWMW -re -kit -i51
del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\SimpleWMW md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\SimpleWMW
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\SimpleWMW\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\SimpleWMW\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\SimpleWMW\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\SimpleWMW\*.dll

