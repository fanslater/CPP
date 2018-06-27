#define RELEASE_VER 1  // 0: beta version; 1: release version
#define RELEASE_DATE "Sep 01 2014" // Mmm dd yyyy; only used for RELEASE_VER=1

#define RELEASE_VER_MAIN  1 // version number (binary)
#define RELEASE_VER_MAIN2 0
#define RELEASE_VER_SUB   0
#define RELEASE_VER_SUB2 100000038 

// version number (string)
#define TOSTRING2(arg) #arg
#define TOSTRING(arg) TOSTRING2(arg)
#define RELEASE_VER_STR     TOSTRING(RELEASE_VER_MAIN) "." TOSTRING(RELEASE_VER_MAIN2) "." TOSTRING(RELEASE_VER_SUB) "." TOSTRING(RELEASE_VER_SUB2))


