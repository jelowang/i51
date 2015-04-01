copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armcc.exe *.exe
copy ..\..\i51TOOL\armcpp.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
semo WheresMyWater.se
i51Compiler WheresMyWater.se >> results.txt
PackageMaker.exe WheresMyWater -re -kit -i51

del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.dll

if not exist ..\..\..\Release\SDK_Alpha_1.3.0\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater md ..\..\..\Release\SDK_Alpha_1.3.0\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater
if exist *.i51 copy *.i51 ..\..\..\Release\SDK_Alpha_1.3.0\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.i51
if exist *.kit copy *.kit ..\..\..\Release\SDK_Alpha_1.3.0\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.kit
if exist *.re copy *.re ..\..\..\Release\SDK_Alpha_1.3.0\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\..\Release\SDK_Alpha_1.3.0\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\WheresMyWater\*.dll

pause