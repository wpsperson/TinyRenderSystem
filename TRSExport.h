/*!  
 *@brief    �����ӿ�
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once

#ifdef TRSLIB
    #define TRS_EXPORT __declspec(dllexport)
#else
    #define TRS_EXPORT __declspec(dllimport)
#endif


