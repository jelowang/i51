copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe

Semo i51KitStd.se
i51Compiler i51KitStd.se

PackageMaker.exe i51KitStd -re -kit -i51

if not exist .\i51KitStd_Set md .\i51KitStd_Set 
copy .\i51KitStd.kit .\i51KitStd_Set\i51KitStd.kit
copy .\i51KitStd.package .\i51KitStd_Set\i51KitStd.package
copy .\Debug\i51KitStd.dll .\i51KitStd_Set\i51KitStd.dll
copy .\i51KitStd.kit ..\PKiGAPI\i51KitStd.kit
copy .\Debug\i51KitStd.dll ..\PKiGAPI\i51KitStd.dll
copy .\i51KitStd.kit ..\..\..\Release\SDK_Alpha_1.3.0\i51KIT\PKiGAPI\i51KitStd.kit
copy .\Debug\i51KitStd.dll ..\..\..\Release\SDK_Alpha_1.3.0\i51KIT\PKiGAPI\i51KitStd.dll

del *.exe
del *.o

pause