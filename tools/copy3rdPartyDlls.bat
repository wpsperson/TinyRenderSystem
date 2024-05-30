@echo off
set OccPath=D:\3rd-party\occ-7.8.0\win64\vc14\bind
set AssimpPath=D:\Code\TinyRenderSystem\ThirdParty\assimp-4.1.0\bin64
set DstPath=D:\Code\TinyRenderSystem\build\bin

for %%a in (TKernel TKMath TKXSBase TKBRep TKG2d TKG3d TKTopAlgo TKGeomBase TKShHealing TKGeomAlgo TKPrim TKLCAF TKXCAF TKDESTEP TKBinXCAF  TKBO TKMesh TKCDF TKV3d TKVCAF TKService TKCAF TKDE TKHLR) do (
    xcopy %OccPath%\%%a.dll    		 %DstPath%\    /e /q /y
)


xcopy %AssimpPath%\assimp-vc140-mt.dll      %DstPath%\    /e /q /y


pause










