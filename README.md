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

---

## 🛠 기술 스택

| 분야 | 기술                                          |
|------|---------------------------------------------|
| **엔진** | Unreal Engine 5.5                           |
| **언어** | C++ (주요 로직), Blueprint (UI/애니메이션)           |
| **입력** | Enhanced Input System                       | 현대적인 입력 처리 |
| **UI** | Common UI + UMG                             | 크로스플랫폼 UI 시스템 |
| **AI** | Behavior Tree, Custom BT Tasks & Decorators |
| **애니메이션** | Motion Warping                              | Animation Notify System |
| **아키텍처** | Subsystem Pattern                           | 모듈화된 시스템 관리 |

---

## 🏗 아키텍처 설계

### 컴포넌트 시스템
```cpp
// 주요 컴포넌트들
UCombatComponent     // 전투 로직
UHealthComponent     // 체력 시스템
UManaComponent       // 마나 시스템
UPhaseComponent      // 보스 페이즈
UCutSceneComponent   // 컷신 재생
```

### Interface 기반 설계
```cpp
// 주요 인터페이스
ISaveGameInterface    // 저장 가능한 객체
IFlying              // 비행 가능 캐릭터
ILoadingScreenInterface  // 로딩 스크린 반응
```

### 스킬 시스템
- **USkillBase**: 모든 스킬의 베이스 클래스
- **Data Asset 기반**: 스킬 설정의 모듈화
- **확장 가능한 구조**: 새로운 스킬 타입 쉽게 추가

```cpp
// 스킬 상속 구조 예시
USkillBase
├── USkillCollisionAttack  // 충돌 기반 스킬
├── USkillProjectile       // 투사체 스킬
└── USkillParrying         // 방어 스킬
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
└── HitStopSubsystem        // 시간 지연 효과
```

---

## 🎯 핵심 기능

### 1. 플레이어 시스템
- **Enhanced Input System**: 현대적인 입력 처리
- **스킬 세트**: 데이터 기반 스킬 세트
- **타겟 록온**: 적 자동 추적 시스템
- **스킬 애니메이션**: Notify를 통한 스킬 발동

### 2. AI 시스템
- **Custom BT Tasks**: 특화된 AI 행동 노드
- **동적 스킬 선택**: 상황에 맞는 스킬 사용
- **적응형 행동**: 플레이어 거리에 따른 행동 변화

### 3. 보스 AI (DesertDragon)
- **페이즈 시스템**: 체력에 따른 행동 패턴 변화
- **Flying Interface**: 비행 상태 관리
- **Motion Warping**: 정확한 타겟팅 공격

### 4. UI 시스템
- **모듈화된 위젯**: 재사용 가능한 UI 컴포넌트
- **실시간 정보**: 체력, 마나, 타겟 정보 표시
- **스킬 프리뷰**: 스킬 아이콘 및 코스트 표시
- **Common UI Framework**: 크로스플랫폼 지원 UI 아키텍처
- **Options System**:
    - 동적 설정 생성 및 관리 (Graphics/Audio/Gameplay)
    - Key Remapping (키보드/게임패드 분리)
    - Edit Condition 기반 설정 의존성 관리

### 5. Component, 상속, Interface
- **Component**: 공통된 행동
- **상속**: 분기화된 행동
- **Interface**: 각각 다른 행동

### 6. 데이터 관리
- **Save/Load System**: SaveGame 기반 저장 시스템
- **SaveGameInterface/Subsystem**: 확장 가능한 중앙 관리 시스템

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

---

## 📁 주요 디렉터리 구조

```
Source/Delta/
├── AI/                 # AI 관련 BT Tasks
├── Animations/         # AnimInstance, Notify
├── AsyncAction/        # 비동기 Blueprint Nodes
├── Characters/         # 캐릭터 클래스들
├── Components/         # 시스템 컴포넌트들
├── Controllers/        # 플레이어/AI/Front 컨트롤러
├── DataAssets/         # 데이터 기반 설정
├── DeltaTypes/         # 게임 네이밍 설정
├── DeveloperSettings/  # 에디터 프로젝트 설정
├── GameUserSettings/   # 런타임 사용자 설정
├── Interfaces/         # 인터페이스들
├── Skills/             # 스킬, Projectile
├── Subsystem/          # 전역 시스템 관리
├── UI/                 # Common UI, HUD
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
    - DeltaNamesapceTypes
    - DeltaStructTypes

### 성능 최적화
- **메모리 관리**:
    - WeakPtr을 통한 순환 참조 방지
    - Soft Reference를 통한 지연 로딩
- **렌더링**:
    - Object Pooling (UI 위젯)
- **AI**:
    - Timer 기반 비동기 처리
    - BehaviorTree Decorator 최적화
---

## 📈 향후 개선 계획

- [ ] 동료 NPC 추가 및 교대 플레이 시스템
- [ ] 절차적 레벨 생성
- [ ] 일반 적 타입

---

## 📞 연락처

**이름**: 이난새
**이메일**: ns1337@naver.com

---

이 프로젝트는 포트폴리오 목적으로 제작되었습니다.

---

*"clean code와 확장 가능한 아키텍처를 통해 고품질 게임 개발 역량을 보여드립니다."*
