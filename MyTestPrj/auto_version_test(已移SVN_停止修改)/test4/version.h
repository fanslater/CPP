#pragma once

#define VER_NO_MAJOR 1 
#define VER_NO_MINOR 0
#define VER_NO_REVISION 0 
#define VER_NO_BUILD 1

#define STR1(R)  #R
#define STR2(R)  STR1(R)
#define VERSION_STR     STR2(VER_NO_MAJOR)"," STR2(VER_NO_MINOR)"," STR2(VER_NO_REVISION)"," STR2(VER_NO_BUILD))


