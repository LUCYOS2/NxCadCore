# NxCadCore

NX 북마크(.plmxml) 오픈 + Assembly Tree/요약 Geometry/Material 읽기를 담당하는 공유 모듈.
원래 [TV_BitSam](https://github.com/LUCYOS2/TV_BitSam) 프로젝트의 `CadImportModule`+`Shared`로
시작했으며, 동일한 "NX 구조 진입" 단계가 필요한 다른 프로젝트에서도 재사용하기 위해 별도
공유 저장소로 분리했다. 소비 프로젝트는 git submodule로 참조한다 (경로: `external/NxCadCore`).

## 구조

```
NxCadCore/
├─ CadImportModule/
│  ├─ Core/        NXOpen 비의존 (인터페이스, 데이터 모델, Logger)
│  ├─ NxBackend/   NXOpen 의존 (NxConnector: 북마크 Open, NxAssemblyReader, NxGeometryReader, NxMaterialReader)
│  ├─ docs/        DevGuide.md
│  └─ tests/
└─ Shared/
   ├─ NxContracts/  INxSessionAccessor(세션 접근 인터페이스), NxGeometryUtils
   └─ NxOpenSdk.props  NX Open Include/Lib 경로 설정
```

## 역할 경계

- **여기서 하는 일**: 북마크 파일 → 실제 부품/어셈블리 참조 해석 → NX Open API로 열기 →
  Assembly Tree/요약 Geometry(Body/Face/Edge 개수, BoundingBox)/Material 이름 읽기
- **여기서 안 하는 일**: 규칙 기반 형상 탐색(Hole/Boss 등 anchor_type을 조건 검색), ROI 지정,
  Facet/Mesh 추출, Ray Tracing — 이런 건 각 소비 프로젝트가 `Shared/NxContracts::INxSessionAccessor`를
  통해 세션에 접근해서 자체 모듈로 구현한다 (TV_BitSam의 RoiModule이 이 패턴의 예시)

## 소비 프로젝트에서 참조하는 법 (submodule)

```
git submodule add https://github.com/LUCYOS2/NxCadCore.git external/NxCadCore
```

vcxproj/MSBuild 기반 프로젝트는 `$(SolutionDir)external\NxCadCore\...` 경로로 include/참조.
CMake 기반 프로젝트는 `add_subdirectory(external/NxCadCore/CadImportModule/Core)` 등으로 연결.

## 빌드 상태

| 프로젝트 | 필요 환경 |
|---|---|
| CadImportModule/Core | 없음 (순수 C++) |
| CadImportModule/NxBackend | NX2406 SDK (회사PC 전용) |

## 원본

TV_BitSam 커밋 히스토리(2a4859f ~ b76f71b)에서 스냅샷으로 분리됨 — 상세 변경 이력은
TV_BitSam 저장소 참고.
