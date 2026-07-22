# CMake 버전의 NxOpenSdk.props (MSBuild 쪽은 NxOpenSdk.props 참고).
# 상위 CMakeLists.txt가 UGII_BASE_DIR 존재 여부로 이 서브디렉토리 자체를
# add_subdirectory 할지 말지 가르므로, 여기서는 존재를 전제하고 그냥 씀.
add_library(NxOpenSdk INTERFACE)
target_include_directories(NxOpenSdk INTERFACE "$ENV{UGII_BASE_DIR}/UGOPEN")
target_link_directories(NxOpenSdk INTERFACE "$ENV{UGII_BASE_DIR}/UGOPEN")
# TODO(office-PC verify): NxOpenSdk.props와 동일 - 정확한 .lib 파일명이 NX2406/2026
# 설치본과 일치하는지 확인 필요.
target_link_libraries(NxOpenSdk INTERFACE libNXOpenCpp.lib libNXOpenCppUI.lib libufun.lib)

add_subdirectory(NxContracts)
