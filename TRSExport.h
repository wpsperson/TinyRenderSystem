/*!  
 *@brief    导出接口
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#pragma warning( disable: 4251 )

#ifdef TRSLIB
    #define TRS_EXPORT __declspec(dllexport)
#else
    #define TRS_EXPORT __declspec(dllimport)
#endif


