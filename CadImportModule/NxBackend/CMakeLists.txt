# CMake 버전의 NxBackend.vcxproj(MSBuild, TV_BitSam용). 실제 NX Open API 호출부라
# UGII_BASE_DIR이 있을 때만(상위에서 add_subdirectory 여부를 가름) 빌드 대상이 된다.
add_library(NxBackend STATIC
    include/NxBackend/NxConnector.h
    include/NxBackend/NxAssemblyReader.h
    include/NxBackend/NxGeometryReader.h
    include/NxBackend/NxMaterialReader.h
    src/NxConnector.cpp
    src/NxAssemblyReader.cpp
    src/NxGeometryReader.cpp
    src/NxMaterialReader.cpp
)

target_include_directories(NxBackend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(NxBackend PUBLIC cxx_std_17)
target_link_libraries(NxBackend PUBLIC NxCadCoreBase NxContracts NxOpenSdk)
