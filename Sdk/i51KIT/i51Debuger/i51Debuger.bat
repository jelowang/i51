copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
i51Compiler i51Debuger.se
PackageMaker.exe i51Debuger -re -kit -i51
del *.exe
del *.o
del *.axf

if not exist ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Debuger md ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Debuger
if exist *.i51 copy *.i51 ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Debuger\*.i51
if exist *.kit copy *.kit ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Debuger\*.kit
if exist *.re copy *.re ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Debuger\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll ..\..\i51EMU\WIN32FS\DRIVE_E\i51\i51KIT\i51Debuger\*.dll

::custom
if not exist .\i51Debuger md .\i51Debuger
if exist *.i51 copy *.i51 .\i51Debuger\*.i51
if exist *.kit copy *.kit .\i51Debuger\*.kit
if exist *.re copy *.re .\i51Debuger\*.re
if exist .\Debug\*.dll copy .\Debug\*.dll .\i51Debuger\*.dll
if exist *.package copy *.package .\i51Debuger\*.package
if exist *.h copy *.h .\i51Debuger\*.h
