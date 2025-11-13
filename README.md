# Delta - Action RPG Game

## 📋 프로젝트 개요

**Delta**는 Unreal Engine 5.5와 C++로 개발한 3D 액션 RPG 게임입니다.
모듈화된 아키텍처와 확장 가능한 시스템 설계에 중점을 두고 개발했습니다.

## ✨ 프로젝트 하이라이트

- 🎯 **모던 언리얼 아키텍처**: Subsystem, Gameplay Tags, Common UI
- 🧩 **확장 가능한 설계**: Interface, Component, Data Asset 기반
- 🎮 **크로스플랫폼 지원**: 키보드/마우스 + 게임패드 완벽 지원
- 💾 **완전한 옵션 시스템**: 그래픽, 사운드, 키 리매핑 등
- 🤖 **스마트 AI**: 거리 기반 스킬 선택, 페이즈 전환
- 👥 **동료 시스템**: 실시간 캐릭터 교체 및 팀 관리
- 🎬 **시네마틱 연출**: 포스트 프로세스, 타임 딜레이션, 컷신

## 🎬 데모 영상

<img width="50%" src="https://github.com/user-attachments/assets/52b1cad1-b74c-41fa-a401-517299db503c"/>


**주요 시연 내용:**
- 다양한 스킬 사용법
- AI와의 전투 시스템
- 보스전 페이즈 변화
- UI 인터랙션

---


### 🎮 주요 특징
- **다양한 스킬 시스템**: 10여 가지 고유한 스킬 (투사체, 근접공격, 방어 등)
- **고급 AI 시스템**: Behavior Tree 기반 적응형 AI
- **실시간 전투**: 타겟팅, 패링 시스템
- **보스 전투**: 페이즈 기반 드래곤 보스 AI
- **동료 시스템**: 캐릭터 전환 및 AI 제어
- **난이도 선택**: Easy/Normal/Hard 3단계 난이도
- **비주얼 피드백**: 데미지 비네트, 카메라 쉐이크, 타임 딜레이션

---

## 🛠 기술 스택

| 분야 | 기술                                          |
|------|---------------------------------------------|
| **엔진** | Unreal Engine 5.5                           |
| **언어** | C++ (주요 로직), Blueprint (UI/애니메이션)           |
| **입력** | Enhanced Input System                       |
| **UI** | Common UI + UMG                             |
| **AI** | Behavior Tree, Custom BT Tasks & Decorators |
| **애니메이션** | Motion Warping, Animation Notify System      |
| **아키텍처** | Subsystem Pattern                           |
| **비주얼** | Post Process, Material Instance Dynamic     |

---

## 🏗 아키텍처 설계~~~~

### 컴포넌트 시스템
```cpp
// 주요 컴포넌트들
UCombatComponent           // 전투 로직
UHealthComponent           // 체력 시스템
UManaComponent             // 마나 시스템
UPhaseComponent            // 보스 페이즈
UCutSceneComponent         // 컷신 재생
UDamageVignetteComponent   // 데미지 비주얼 피드백
```

### Interface 기반 설계
```cpp
// 주요 인터페이스
ISaveGameInterface         // 저장 가능한 객체
IFlying                    // 비행 가능 캐릭터
ILoadingScreenInterface    // 로딩 스크린 반응
```

### 캐릭터 시스템
```cpp
// 캐릭터 상속 구조
ADeltaBaseCharacter          // 베이스 캐릭터 (공통 기능)
├── ADeltaPlayableCharacter  // 플레이 가능 캐릭터 (플레이어/동료)
└── ADeltaEnemyCharacter     // 적 캐릭터
    └── ADesertDragonCharacter  // 보스 캐릭터
```

### 컨트롤러 시스템
```cpp
// 컨트롤러 구조
ADeltaPlayerController       // 메인 플레이어 제어
ADeltaAIController           // AI 베이스 컨트롤러
├── ADeltaAllyController     // 동료 AI 컨트롤러
└── ADeltaEnemyController    // 적 AI 컨트롤러
```

### 스킬 시스템
- **USkillBase**: 모든 스킬의 베이스 클래스
- **Data Asset 기반**: 스킬 설정의 모듈화
- **확장 가능한 구조**: 새로운 스킬 타입 쉽게 추가

```cpp
// 스킬 상속 구조
USkillBase
├── USkillCollisionAttack  // 충돌 기반 스킬
├── USkillProjectile       // 투사체 스킬
│   ├── USkillArrowShot
│   ├── USkillFlyingDisk
│   └── USkillSpreadFire
├── USkillParrying         // 방어 스킬
├── USkillTreeBloom        // 범위 공격
├── USkillForestBoom       // 폭발 스킬
└── USkillBlowAway         // 넉백 스킬
```

### Subsystem 기반 설계
```cpp
// 게임 전역 시스템을 Subsystem으로 관리
GameInstance Subsystems:
├── AudioSubsystem          // 사운드 관리
├── SaveGameSubsystem       // 저장/로드
├── FrontUISubsystem        // 메인 UI 관리
└── LoadingScreenSubsystem  // 로딩 화면

World Subsystems:
└── HitStopSubsystem        // 시간 지연 효과 (타임 딜레이션)
```

---

## 🎯 핵심 기능

### 1. 플레이어 시스템
- **Enhanced Input System**: 현대적인 입력 처리
- **스킬 세트**: 데이터 기반 스킬 세트
- **타겟 록온**: 적 자동 추적 시스템
- **스킬 애니메이션**: Notify를 통한 스킬 발동
- **카메라 시스템**: 동적 줌, 타겟 팔로우, 카메라 쉐이크

### 2. 동료 시스템
- **캐릭터 전환**: 실시간으로 파티 멤버 간 교체
- **AI 제어**: 플레이하지 않는 동료는 AI가 자동 제어
- **팀 관리**: 동적 팀 멤버 등록/해제 시스템
- **상태 유지**: 캐릭터 전환 시 스킬 세트, 카메라 설정 저장

### 3. AI 시스템
- **Custom BT Tasks**: 특화된 AI 행동 노드
  - BTTask_AttackTarget: 타겟 공격
  - BTTask_PlayerUpdate: 플레이어 추적
  - BTTask_SetLocationFromTarget: 타겟 기반 위치 설정
  - BTTask_WaitAttackDelay: 공격 딜레이 관리
- **Custom BT Decorators**: 조건 확인 노드
  - BTDecorator_IsTargetClose: 타겟 근접 확인
  - BTDecorator_IsPlayerTooFar: 플레이어 거리 확인
  - BTDecorator_NeedCollectMana: 마나 수집 필요 확인
- **동적 스킬 선택**: 상황에 맞는 스킬 사용
- **적응형 행동**: 플레이어 거리에 따른 행동 변화
- **EQS 시스템**: 비행 위치, 플레이어 팔로우 등

### 4. 보스 AI (DesertDragon)
- **페이즈 시스템**: 체력에 따른 행동 패턴 변화
- **Flying Interface**: 비행 상태 관리
- **Motion Warping**: 정확한 타겟팅 공격
- **다양한 공격 패턴**: 지상 / 비행 각각의 패턴

### 5. 난이도 시스템 (Difficulty System)
- **3단계 난이도**: Easy, Normal, Hard
- **동적 밸런싱**: 적 체력, 공격력 조정
- **저장/로드 지원**: 난이도 설정 저장

### 6. 비주얼 피드백 시스템
- **Damage Vignette**: 체력에 따른 화면 가장자리 어두워짐
- **Camera Shake**: 피격 시 카메라 흔들림
- **Time Dilation**: 스킬 사용 시 시간 느려짐 효과
- **Post Process**: 동적 포스트 프로세스 효과
- **Hit React**: 방향별 피격 애니메이션 (전/후/좌/우)

### 7. UI 시스템
- **모듈화된 위젯**: 재사용 가능한 UI 컴포넌트
- **실시간 정보**: 체력, 마나, 타겟 정보 표시
- **스킬 프리뷰**: 스킬 아이콘 및 코스트 표시
- **팀 정보 표시**: 파티 멤버 체력 및 상태 표시
- **Common UI Framework**: 크로스플랫폼 지원 UI 아키텍처
- **Options System**:
    - 동적 설정 생성 및 관리 (Graphics/Audio/Gameplay)
    - Key Remapping (키보드/게임패드 분리)
    - 난이도 선택
    - Edit Condition 기반 설정 의존성 관리

### 8. Component, 상속, Interface
- **Component**: 공통된 행동
  - CombatComponent: 전투 기능
  - HealthComponent: 체력 관리
  - ManaComponent: 마나 관리
- **상속**: 분기화된 행동
  - BaseCharacter → PlayableCharacter → 특정 플레이어 캐릭터
  - BaseCharacter → EnemyCharacter → 특정 적 캐릭터
- **Interface**: 각각 다른 행동
  - ISaveGameInterface: 저장 기능
  - IFlying: 비행 기능

### 9. 데이터 관리
- **Save/Load System**: SaveGame 기반 저장 시스템
- **SaveGameInterface/Subsystem**: 확장 가능한 중앙 관리 시스템
- **저장 항목**:
  - 캐릭터 상태 (위치, 체력, 마나)
  - 난이도 설정
  - 게임 진행 시간

---

## 💡 기술적 도전과 해결

### 1. 확장 가능한 스킬 시스템
**문제**: 다양한 스킬 타입을 통합 관리하면서 확장성 확보 

**해결**:
- UObject를 기반으로 한 SkillBase 설계
- 상속 기반 스킬 클래스 설계
- Data Asset을 통한 설정 외부화
- Component 패턴으로 재사용성 극대화

### 2. 복잡한 AI 행동 관리
**문제**: 다양한 AI 상태와 조건 처리

**해결**:
- Custom BT Decorator/Task 제작
- 캐릭터 기반 행동 분리
- 동료와 적 AI 컨트롤러 분리 설계

### 3. 실시간 타겟팅 시스템
**문제**: 부드러운 카메라 전환과 정확한 타겟팅

**해결**:
- Motion Warping으로 정밀한 애니메이션
- Multi Trace를 통한 최적 타겟 선택
- Lerp 기반 부드러운 카메라 제어

### 4. 애니메이션과 스킬의 연결
**문제**: 몽타주 디버깅의 어려움 및 역할 분리

**해결**:
- AnimMontage - Notify - BaseCharacter - Skill 구조를 통해 역할 분리
- Notify 단순 호출 기능, Character 제어, Skill 구현 으로 분리해 문제 발생 시 파악을 간단하게 함
- AnimNotifyState를 통한 스킬 타이밍 제어

### 5. 크로스플랫폼 UI 시스템
**문제**: 키보드/마우스와 게임패드 모두 지원하는 설정 화면

**해결**:
- Common UI의 Input Routing 활용
- 동적 Widget Focus 관리
- Input Type별 Key Remapping 분리

### 6. Save/Load 시스템
**문제**: 다양한 액터 타입의 통일된 저장

**해결**:
- Interface 기반 설계
- Subsystem을 통한 중앙 관리
- 캐릭터, 컨트롤러, 게임 모드 모두 ISaveGameInterface 구현

### 7. 동료 시스템 (Ally System)
**문제**: 실시간 캐릭터 전환과 AI 제어 전환

**해결**:
- PlayableCharacter를 플레이어와 동료 모두에 사용
- Controller 교체를 통한 제어권 전환 (PlayerController ↔ AllyController)
- 상태 캐싱을 통한 매끄러운 전환
- 팀 멤버 등록/해제 시스템

### 8. 비주얼 피드백 시스템
**문제**: 다양한 비주얼 효과를 통합 관리

**해결**:
- Component 기반 DamageVignette 설계
- Material Instance Dynamic을 통한 실시간 파라미터 제어
- Subsystem 기반 타임 딜레이션 관리
- PostProcessHolder 액터를 통한 포스트 프로세스 중앙화

### 8. 리소스 공유 시스템
**문제**: BehaviorTree, AnimNotifyState 의 상태 공유

**해결**:
- BehaviorTree, AnimNotifyState 가 아닌 캐릭터에 저장
- BehaviorTree 에 저장해야 하면 NodeMemory 사용

---

## 📁 주요 디렉터리 구조

```
Source/Delta/
├── AI/                      # AI 관련 BT Tasks, Decorators
│   ├── BTTask_AttackTarget
│   ├── BTTask_PlayerUpdate
│   ├── BTTask_SetLocationFromTarget
│   ├── BTDecorator_IsTargetClose
│   └── BTDecorator_IsPlayerTooFar
├── Animations/              # AnimInstance, Notify
│   └── Notify/
│       ├── AnimNotifyState_SkillTimeDilation
│       └── AnimNotify_ActiveSkill
├── AsyncAction/             # 비동기 Blueprint Nodes
├── Characters/              # 캐릭터 클래스들
│   ├── DeltaBaseCharacter
│   ├── DeltaPlayableCharacter
│   └── Enemy/
│       ├── DeltaEnemyCharacter
│       └── DesertDragonCharacter
├── Components/              # 시스템 컴포넌트들
│   ├── CombatComponent
│   ├── HealthComponent
│   ├── ManaComponent
│   ├── PhaseComponent
│   ├── CutSceneComponent
│   └── DamageVignetteComponent
├── Controllers/             # 컨트롤러
│   ├── DeltaPlayerController
│   ├── DeltaAIController
│   ├── DeltaAllyController
│   └── DeltaEnemyController
├── DataAssets/              # 데이터 기반 설정
│   ├── Skill/
│   ├── Input/
│   └── UI/
├── DeltaTypes/              # 게임 타입 정의
│   ├── DeltaEnumTypes (난이도, 팀 소속 등)
│   ├── DeltaNamespaceTypes
│   └── DeltaStructTypes
├── DeveloperSettings/       # 에디터 프로젝트 설정
├── GameUserSettings/        # 런타임 사용자 설정
├── Interfaces/              # 인터페이스들
│   ├── SaveGameInterface
│   ├── Flying
│   └── LoadingScreenInterface
├── PostProcess/             # 포스트 프로세스
│   └── PostProcessHolder
├── Skills/                  # 스킬, Projectile
│   ├── SkillBase
│   ├── SkillProjectile
│   ├── SkillCollisionAttack
│   └── Projectiles/
├── Subsystem/               # 전역 시스템 관리
│   ├── AudioSubsystem
│   ├── SaveGameSubsystem
│   ├── FrontUISubsystem
│   ├── LoadingScreenSubsystem
│   └── HitStopSubsystem
└── UI/                      # Common UI, HUD
    ├── DeltaHUD
    ├── Options/
    └── Plays/
        ├── PlayWidget
        ├── SkillInfoWidget
        └── TargetInfoWidget
```

---

## 🔧 개발 도구 & 최적화

### 개발자 편의 기능
- **Developer Settings**: 게임 설정의 중앙화된 관리
    - AudioDeveloperSettings
    - FrontDeveloperSettings
    - LoadingScreenDeveloperSettings
- **Data Assets**: 데이터 기반 설정
    - SkillDataAsset
    - InputDataAsset
    - UIDataAsset
- **Delta Types**: 네이밍 등의 중앙화 관리
    - DeltaEnumTypes
    - DeltaNamespaceTypes
    - DeltaStructTypes

### 성능 최적화
- **메모리 관리**:
    - WeakPtr을 통한 순환 참조 방지
    - Soft Reference를 통한 지연 로딩
- **렌더링**:
    - Object Pooling (UI 위젯)
    - Material Instance Dynamic 캐싱
- **AI**:
    - Timer 기반 비동기 처리
    - BehaviorTree Decorator 최적화
    - EQS 쿼리 최적화
- **게임플레이**:
    - Component 기반 모듈화로 불필요한 업데이트 최소화
    - Subsystem을 통한 효율적인 전역 관리

---

## 📈 향후 개선 계획

- [x] 동료 NPC 추가 및 교대 플레이 시스템
- [x] 난이도 시스템 구현
- [x] 비주얼 피드백 강화 (데미지 비네트, 타임 딜레이션)
- [ ] 절차적 레벨 생성
- [ ] 플레이어블 캐릭터 추가
- [ ] 멀티플레이어 지원 <- 고민 중

---

## 🎓 배운 점

### 아키텍처 설계
- **Subsystem 패턴**의 강력함을 체감
  - 전역 시스템을 깔끔하게 관리
  - 의존성 주입이 쉬워짐
- **Interface vs Component vs 상속**의 적절한 사용
  - Interface: 완전히 다른 구현 (Flying, SaveGame)
  - Component: 공통 기능 (Combat, Health)
  - 상속: 점진적 특화 (Character 계층)

### AI 개발
- **Custom BT Task/Decorator**의 중요성
  - 재사용 가능한 AI 행동 블록
  - 디버깅과 유지보수가 용이
- **EQS**를 통한 지능적인 위치 선택

### 게임플레이 프로그래밍
- **Data Asset**의 효율성
  - 코드 수정 없이 콘텐츠 추가
  - 디자이너 친화적
- **Animation Notify**를 통한 게임플레이 타이밍 제어

---

## 📞 연락처

**이름**: 이난새
**이메일**: ns1337@naver.com

---

이 프로젝트는 포트폴리오 목적으로 제작되었습니다.

---

*"clean code와 확장 가능한 아키텍처를 통해 고품질 게임 개발 역량을 보여드립니다."*
