# NX Open 세션에 의존하는 공유 헬퍼(BodiesOfComponent 등) - CadImportModule/NxBackend와
# TV_BitSam의 RoiModule/NxBackend가 함께 쓴다. NX SDK 없이는 빌드 안 되므로 이 타겟은
# UGII_BASE_DIR이 있을 때만 add_subdirectory 된다 (상위 Shared/CMakeLists.txt 참고).
add_library(NxContracts STATIC
    include/NxContracts/INxSessionAccessor.h
    include/NxContracts/NxGeometryUtils.h
    src/NxGeometryUtils.cpp
)

target_include_directories(NxContracts PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# NxGeometryUtils.cpp가 자기 헤더를 "NxContracts/" 접두사 없이 "NxGeometryUtils.h"로
# include하고 있어서(NxBackend 쪽 파일들과 스타일이 다름), 이 타겟 내부 컴파일에서만
# include/NxContracts 자체도 검색 경로에 추가한다.
target_include_directories(NxContracts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/NxContracts)
target_compile_features(NxContracts PUBLIC cxx_std_17)
# NxGeometryUtils.h가 CadImport::Core::BoundingBox3D(Core/Models/BoundingBox3D.h)를
# 쓰므로 NxCadCoreBase의 include 경로가 필요하다.
target_link_libraries(NxContracts PUBLIC NxOpenSdk NxCadCoreBase)
