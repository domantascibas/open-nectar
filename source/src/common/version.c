#include <stdio.h>
#include "version.h"

#define NS_POWER

#define RELEASE_VER             "00.01.10.00"

#ifdef NS_POWER
#define DEV_MODEL_STR           "NS-Power"
#define HW_SIGN_ARRAY           { 0x50,0x77,0x30,0x31 }     // Pw01
#define RELEASE_VERSION_STR     RELEASE_VER
#endif

#ifdef NS_MAIN
#define DEV_MODEL_STR           "NS-Main"
#define HW_SIGN_ARRAY           { 0x4D,0x6E,0x30,0x31 }     // Mn01
#define RELEASE_VERSION_STR     RELEASE_VER
#endif

const char Dev_Model[DEV_NAME_LEN] = DEV_MODEL_STR;
const char HW_sign[4] = HW_SIGN_ARRAY;
const char version[12] = RELEASE_VERSION_STR;