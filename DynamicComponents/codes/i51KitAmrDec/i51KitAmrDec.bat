copy ..\..\i51TOOL\tcc.exe *.exe
copy ..\..\i51TOOL\armlink.exe *.exe
copy ..\..\i51TOOL\armcc.exe *.exe
copy ..\..\i51TOOL\armcpp.exe *.exe
copy ..\..\i51TOOL\i51Compiler.exe *.exe
copy ..\..\i51TOOL\PackageMaker.exe *.exe
copy ..\..\i51TOOL\Semo.exe *.exe
Semo i51KitAmrDec.se
i51Compiler i51KitAmrDec.se
PackageMaker.exe i51KitAmrDec -re -kit -i51
del *.exe
del *.o
del *.axf



