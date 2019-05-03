/*!  
 *@brief    导出接口
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once

#ifdef TRSLIB
    #define TRS_EXPORT __declspec(dllexport)
#else
    #define TRS_EXPORT __declspec(dllimport)
#endif


