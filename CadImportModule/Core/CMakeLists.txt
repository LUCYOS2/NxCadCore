# CMake 빌드 정의. Core.vcxproj(MSBuild, TV_BitSam용)와 별개로, CMake 기반
# 소비 프로젝트(NX_CrossCompare 등)를 위해 추가했다. 파일 목록은 Core.vcxproj와
# 동일하게 유지할 것 - 한쪽에 파일을 추가하면 다른 쪽도 같이 갱신 필요.
add_library(NxCadCoreBase STATIC
    include/Core/Interfaces/IAssemblyReader.h
    include/Core/Interfaces/IGeometryReader.h
    include/Core/Interfaces/ILogger.h
    include/Core/Interfaces/IMaterialReader.h
    include/Core/Interfaces/INxSessionProvider.h
    include/Core/Logging/ConsoleLogger.h
    include/Core/Models/BoundingBox3D.h
    include/Core/Models/ComponentInfo.h
    include/Core/Models/GeometryInfo.h
    include/Core/Models/MaterialInfo.h
    include/Core/Models/OperationResult.h
    include/Core/Models/SessionInfo.h
    src/ConsoleLogger.cpp
)

target_include_directories(NxCadCoreBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(NxCadCoreBase PUBLIC cxx_std_17)
