#pragma once

#define STR1(R)  #R
#define STR2(R)  STR1(R)

#define MAJOR_NUM       1         //���汾
#define MINOR_NUM       0         //�ΰ汾
#define REVISION_NUM    0      //�޶��汾

#define FILE_MAJOR MAJOR_NUM
#define FILE_MINOR MINOR_NUM
#define FILE_REVISION REVISION_NUM
#define FILE_BUILD 4            //build���windowsĬ����ʾ����Ϊ65535=2^16-1��
#define FILE_VERSION_STR    STR2(FILE_MAJOR) ", " STR2(FILE_MINOR) ", " STR2(FILE_REVISION) ", " STR2(FILE_BUILD) 

#define PRODUCT_MAJOR MAJOR_NUM
#define PRODUCT_MINOR MINOR_NUM
#define PRODUCT_REVISION REVISION_NUM
#define PRODUCT_BUILD 20180711174306
#define PRODUCT_VERSION_STR    STR2(PRODUCT_MAJOR) ", " STR2(PRODUCT_MINOR) ", " STR2(PRODUCT_REVISION) ", " STR2(PRODUCT_BUILD) 


