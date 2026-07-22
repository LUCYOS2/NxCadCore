# NX Open 세션에 의존하는 공유 헬퍼(BodiesOfComponent 등) - CadImportModule/NxBackend와
# TV_BitSam의 RoiModule/NxBackend가 함께 쓴다. NX SDK 없이는 빌드 안 되므로 이 타겟은
# UGII_BASE_DIR이 있을 때만 add_subdirectory 된다 (상위 Shared/CMakeLists.txt 참고).
add_library(NxContracts STATIC
    include/NxContracts/INxSessionAccessor.h
    include/NxContracts/NxGeometryUtils.h
    src/NxGeometryUtils.cpp
)

target_include_directories(NxContracts PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(NxContracts PUBLIC cxx_std_17)
target_link_libraries(NxContracts PUBLIC NxOpenSdk)
