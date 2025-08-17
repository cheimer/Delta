# Delta - Action RPG Game

## 📋 프로젝트 개요

**Delta**는 Unreal Engine 5.5와 C++로 개발한 3D 액션 RPG 게임입니다. 
모듈화된 아키텍처와 확장 가능한 시스템 설계에 중점을 두고 개발했습니다.

### 🎮 주요 특징
- **다양한 스킬 시스템**: 10여 가지 고유한 스킬 (투사체, 근접공격, 방어 등)
- **고급 AI 시스템**: Behavior Tree 기반 적응형 AI
- **실시간 전투**: 타겟팅, 패링 시스템
- **보스 전투**: 페이즈 기반 드래곤 보스 AI

---

## 🛠 기술 스택

| 분야 | 기술 |
|------|------|
| **엔진** | Unreal Engine 5.5 |
| **언어** | C++ (주요 로직), Blueprint (UI/애니메이션) |
| **AI** | Behavior Tree, Custom BT Tasks |
| **애니메이션** | Motion Warping, Animation Notify |
| **UI** | UMG, Slate |

---

## 🏗 아키텍처 설계

### 컴포넌트 시스템
```cpp
// 주요 컴포넌트들
UCombatComponent    // 전투 로직 관리
UHealthComponent    // 체력 시스템
UManaComponent      // 마나 시스템  
UPhaseComponent     // 보스 페이즈 관리
```

### 스킬 시스템
- **USkillBase**: 모든 스킬의 베이스 클래스
- **Data Asset 기반**: 스킬 설정의 모듈화
- **확장 가능한 구조**: 새로운 스킬 타입 쉽게 추가

```cpp
// 스킬 상속 구조 예시
USkillBase
├── USkillCollisionAttack  // 근접 공격
├── USkillProjectile       // 투사체 스킬
│   └── USkillSpirFire     // 투사체 스킬 
└── USkillParrying         // 방어 스킬
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

### 5. Component, 상속, Interface
- **Component**: 공통된 행동
- **상속**: 분기화된 행동
- **Interface**: 각각 다른 행동

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
- 다중 Trace를 통한 최적 타겟 선택
- Lerp 기반 부드러운 카메라 제어

### 4. 애니메이션과 스킬의 연결
**문제**: 몽타주 디버깅의 어려움 및 역할 분리
**해결**:
- AnimMontage - Notify - BaseCharacter - Skill 구조를 통해 역할 분리
- Notify 단순 호출 기능, Character 제어, Skill 구현 으로 분리해 문제 발생 시 파악을 간단하게 함

---

## 📁 주요 디렉터리 구조

```
Source/Delta/
├── AI/                 # AI 관련 BT Tasks
├── Characters/         # 캐릭터 클래스들
│   ├── DeltaBaseCharacter
│   ├── DeltaPlayableCharacter  
│   └── Enemy/
├── Components/         # 시스템 컴포넌트들
├── Controllers/        # 플레이어/AI 컨트롤러
└── DataAssets/         # 게임 데이터 설정
├── Skills/             # 스킬 시스템
├── UI/                 # 사용자 인터페이스
```

---

## 📊 성능 최적화

- **메모리 관리**: WeakPtr 활용으로 순환 참조 방지
- **AI**: Timer 기반 비동기 처리로 프레임 드롭 방지
- **UI**: Object Pooling으로 가비지 컬렉션 최소화

---

## 🔧 개발 도구

### 디버깅 기능
- 스킬 데미지 시각화
- AI 상태 실시간 모니터링
- 성능 프로파일링

### 개발자 편의 기능
- 에디터 내 스킬 테스트 도구
- 자동화된 빌드 시스템
- Git 기반 버전 관리

---

## 📈 향후 개선 계획

- [ ] 더 많은 적 추가
- [ ] 절차적 레벨 생성
- [ ] 세이브 & 로드 추가
- [ ] 시작 & 끝 추가

---

## 📞 연락처

**이름**: 이난새
**이메일**: ns1337@naver.com

---

이 프로젝트는 포트폴리오 목적으로 제작되었습니다.

---

*"clean code와 확장 가능한 아키텍처를 통해 고품질 게임 개발 역량을 보여드립니다."*
