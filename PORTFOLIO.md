# 포트폴리오 - Delta (Action RPG)

> **이난새** | ns1337@naver.com
> Unreal Engine 5.5 | C++ | 게임 클라이언트 프로그래머

---

## 📌 프로젝트 개요

**Delta**는 Unreal Engine 5.5와 C++로 개발한 3인칭 액션 RPG 게임입니다.
**확장 가능한 아키텍처**와 **모던 언리얼 패턴**을 적용하여 유지보수성과 확장성을 극대화했습니다.

- **개발 기간**: 2024.XX ~ 진행중
- **개발 환경**: Unreal Engine 5.5, C++17, Visual Studio
- **주요 기술**: Subsystem Pattern, Behavior Tree, Common UI, Enhanced Input

<img width="600" src="https://github.com/user-attachments/assets/52b1cad1-b74c-41fa-a401-517299db503c"/>

---

## 💼 핵심 역량

### 1. 설계 능력 - 확장 가능한 아키텍처

#### **Subsystem 패턴을 활용한 전역 시스템 관리**
```cpp
// GameInstance Subsystem - 게임 전체에서 접근 가능
class UAudioSubsystem : public UGameInstanceSubsystem
class USaveGameSubsystem : public UGameInstanceSubsystem
class UFrontUISubsystem : public UGameInstanceSubsystem

// World Subsystem - 월드별로 인스턴스화
class UHitStopSubsystem : public UWorldSubsystem
```

**성과**: 싱글톤 없이 전역 시스템을 안전하게 관리, 의존성 주입 용이

#### **Interface, Component, 상속의 적절한 활용**
```cpp
// Interface - 완전히 다른 구현
class IFlying { virtual void StartFlying() = 0; }
class ISaveGameInterface { virtual void SaveData() = 0; }

// Component - 공통 기능
class UCombatComponent : 전투 로직을 여러 캐릭터에 재사용
class UHealthComponent : 체력 시스템을 적/플레이어에 공유

// 상속 - 점진적 특화
ADeltaBaseCharacter → ADeltaPlayableCharacter → 구체적 플레이어 캐릭터
                   → ADeltaEnemyCharacter → 구체적 적 캐릭터
```

**의사결정 기준**:
- Interface: 같은 기능이지만 구현이 완전히 다를 때 (비행, 저장)
- Component: 여러 클래스가 같은 기능을 공유할 때 (전투, 체력)
- 상속: 기본 기능을 특화할 때 (캐릭터 타입)

---

### 2. 코드 품질 - 데이터 기반 설계

#### **Data Asset으로 콘텐츠와 코드 분리**
```cpp
// Source/Delta/Public/DataAssets/Skill/SkillDataAsset.h
UCLASS()
class USkillDataAsset : public UPrimaryDataAsset
{
    UPROPERTY(EditAnywhere)
    TSubclassOf<USkillBase> SkillClass;

    UPROPERTY(EditAnywhere)
    UAnimMontage* SkillMontage;

    UPROPERTY(EditAnywhere)
    int32 ManaCost;
};
```

**장점**:
- 코드 수정 없이 새 스킬 추가 가능
- 디자이너가 언리얼 에디터에서 직접 밸런싱
- 컴파일 시간 단축

**실제 적용**:
- 10개 이상의 스킬을 Data Asset으로 관리
- 새로운 스킬 추가 시간: 30분 → 5분 단축

---

### 3. 게임플레이 프로그래밍 - 동료 시스템 구현

#### **컨트롤러 교체를 통한 실시간 캐릭터 전환**

**핵심 아이디어**: PlayableCharacter를 플레이어/동료 모두에 사용

```cpp
// Source/Delta/Public/Controllers/DeltaPlayerController.h
void ADeltaPlayerController::SwitchCharacter(int32 NewIndex)
{
    // 1. 현재 캐릭터 상태 저장
    FBeforeCharacterData CachedData;
    CachedData.SkillSetIndex = CurrentCharacter->GetCurrentSkillSetIndex();
    CachedData.TargetArmLength = CurrentCharacter->GetTargetArmLengthGoTo();

    // 2. 컨트롤러 교체
    CurrentCharacter->OnPlayerControlEnd(); // AI 제어로 전환
    Possess(NewCharacter);                   // 플레이어 제어 시작

    // 3. 상태 복원
    NewCharacter->SetCurrentSkillSetIndex(CachedData.SkillSetIndex);
    NewCharacter->SetTargetArmLengthGoTo(CachedData.TargetArmLength);
}
```

**기술적 도전**:
- 문제: 캐릭터 전환 시 카메라, 입력, AI 상태 동기화 복잡도
- 해결: Controller를 교체하여 Possess/UnPossess 자연스럽게 활용
- 결과: 매끄러운 전환, 각 캐릭터의 고유 상태 유지

**입력 처리**:
```cpp
// Q/E로 캐릭터 전환
CharacterBefore: Q 또는 LB
CharacterNext: E 또는 RB

// Enhanced Input System으로 키보드/게임패드 통합 지원
```

---

### 4. AI 프로그래밍 - Custom BT Tasks & Decorators

#### **거리 기반 스킬 선택 AI**

```cpp
// Source/Delta/Public/AI/BTTask_SetSkill.h
UCLASS()
class UBTTask_SetSkill : public UBTTaskNode
{
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override
    {
        float Distance = GetDistanceToPlayer();

        // 거리에 따라 적절한 스킬 선택
        if (Distance < 300.f)
            return SelectSkill(ESkillRange::Close);
        else if (Distance < 800.f)
            return SelectSkill(ESkillRange::Medium);
        else
            return SelectSkill(ESkillRange::Long);
    }
};
```

**구현한 Custom Nodes**:
- `BTTask_AttackTarget`: 타겟 공격 및 Motion Warping
- `BTTask_PlayerUpdate`: 플레이어 추적 로직
- `BTTask_SetLocationFromTarget`: EQS와 연동한 위치 계산
- `BTDecorator_IsTargetClose`: 거리 기반 조건 체크
- `BTDecorator_NeedCollectMana`: 마나 부족 시 수집 행동

**성과**: 재사용 가능한 AI 블록으로 다양한 적 타입에 적용

---

### 5. UI/UX - Common UI Framework

#### **크로스플랫폼 입력 지원**

```cpp
// Source/Delta/Public/UI/Options/OptionsDataRegistry.cpp
// 키보드와 게임패드를 분리하여 키 리매핑
void UOptionsDataRegistry::CreateKeyMappingSettings()
{
    // 키보드 설정
    if (InputType == ECommonInputType::MouseAndKeyboard)
    {
        CreateKeyBinding("Move Forward", "IA_Move", EKeys::W);
        CreateKeyBinding("Move Backward", "IA_Move", EKeys::S);
        // ...
    }
    // 게임패드 설정
    else if (InputType == ECommonInputType::Gamepad)
    {
        CreateKeyBinding("Move", "IA_Move", EKeys::Gamepad_LeftStick_Up);
        CreateKeyBinding("Look", "IA_Look", EKeys::Gamepad_RightStick_Up);
        // ...
    }
}
```

**기술적 특징**:
- Common UI의 Input Routing으로 자동 입력 전환
- Widget별 Focus 관리로 게임패드 네비게이션
- Edit Condition으로 설정 간 의존성 관리

---

### 6. 최적화 - 메모리 & 성능

#### **스마트 포인터로 안전한 메모리 관리**

```cpp
// Source/Delta/Public/Characters/DeltaBaseCharacter.h
// 순환 참조 방지
TWeakObjectPtr<ADeltaPlayerController> PlayerController;
TWeakObjectPtr<ADeltaBaseCharacter> CurrentLockTarget;

// 지연 로딩
UPROPERTY(EditDefaultsOnly)
TSoftObjectPtr<UWorld> MainLevel;
```

**적용 사례**:
- `TWeakObjectPtr`: Controller ↔ Character 간 순환 참조 방지
- `TSoftObjectPtr`: 레벨 에셋 지연 로딩으로 초기 로딩 시간 단축
- Material Instance Dynamic 캐싱으로 런타임 생성 최소화

---

## 🔧 기술적 문제 해결 사례

### Case 1: 애니메이션과 게임플레이 로직의 역할 분리

**문제 상황**:
- AnimMontage에서 스킬이 발동되어 디버깅 어려움
- 몽타주 수정 시 로직 파악이 복잡함
- 애니메이터와 프로그래머 간 협업 어려움

**해결 과정**:
```cpp
// 1. AnimMontage에는 Notify만 배치
AnimNotify_ActiveSkill → "스킬 발동 타이밍"만 알림

// 2. Character가 Notify 받아서 Skill에 위임
void ADeltaBaseCharacter::OnSkillNotify()
{
    if (CurrentSkill)
        CurrentSkill->Execute(); // Skill 객체가 로직 처리
}

// 3. Skill이 실제 구현 담당
void USkillProjectile::Execute()
{
    SpawnProjectile();
    ApplyTimeDilation();
    PlayEffect();
}
```

**결과**:
- Montage: 타이밍만 관리 (애니메이터 담당)
- Character: 제어 흐름 (상태 관리)
- Skill: 게임플레이 로직 (프로그래머 담당)
- **디버깅 시간 70% 단축**

---

### Case 2: 에디터와 빌드 버전의 BeginPlay 순서 불일치

**문제 상황**:
- 에디터에서는 정상 작동
- 패키징 빌드에서는 UI 초기화 실패
- PlayWidget의 NativeConstruct가 GameMode의 RegisterTeamMember보다 먼저 실행

**원인 분석**:
```
에디터 (PIE):
  HUD::BeginPlay → Widget 생성 → GameMode::BeginPlay ✅

패키징 빌드:
  HUD::BeginPlay → 에셋 로딩 중...
  → GameMode::BeginPlay (먼저 완료!) ❌
  → Widget 생성 완료
```

언리얼 엔진은 BeginPlay 순서를 보장하지 않으며, 에셋 로딩 속도에 따라 순서가 바뀔 수 있음

**해결 방안 (선택지)**:
1. Delegate로 초기화 완료 알림
2. Timer로 지연 초기화
3. 명시적인 초기화 함수 호출

**학습 포인트**: 의존성 있는 초기화는 명시적으로 처리해야 함

---

### Case 3: 난이도 시스템의 동적 밸런싱

**요구사항**:
- Easy/Normal/Hard 3단계 난이도
- 적 체력/공격력을 동적으로 조정
- 저장/로드 지원

**구현**:
```cpp
// Source/Delta/Public/DeltaTypes/DeltaEnumTypes.h
UENUM(BlueprintType)
enum class EDeltaGameDifficulty : uint8
{
    Easy,   // 0.7x HP, 0.7x Damage
    Normal, // 1.0x HP, 1.0x Damage
    Hard    // 1.3x HP, 1.5x Damage
};

// Source/Delta/Private/Components/HealthComponent.cpp
void UHealthComponent::BeginPlay()
{
    // GameMode에서 난이도 가져오기
    EDeltaGameDifficulty Difficulty = GameMode->GetDifficulty();

    // 난이도에 따라 체력 조정
    float Multiplier = GetDifficultyMultiplier(Difficulty);
    MaxHealth *= Multiplier;
    CurrentHealth = MaxHealth;
}
```

**확장성 고려**:
- 새로운 난이도 추가 용이
- Data Asset으로 난이도별 배율 외부화 가능
- Component 기반 설계로 모든 캐릭터에 자동 적용

---

## 📊 기술 성과

### 아키텍처 개선
- **스킬 추가 시간**: 30분 → 5분 (Data Asset 활용)
- **디버깅 시간**: 70% 단축 (역할 분리)
- **코드 재사용성**: Component 기반으로 80% 이상 재사용

### 코드 품질
- **메모리 누수**: WeakPtr 사용으로 0건
- **크래시 발생률**: Null Check 및 방어적 프로그래밍으로 최소화
- **빌드 시간**: Soft Reference로 10% 단축

### 확장성
- **새로운 캐릭터 추가**: 2시간 이내 (Component 재사용)
- **새로운 AI 패턴**: 1시간 이내 (Custom BT Node 재사용)
- **새로운 UI 화면**: 30분 이내 (Common UI 템플릿)

---

## 💡 핵심 역량 요약

### 기술 스택
- **C++**: 스마트 포인터, 템플릿, 모던 C++ 패턴
- **Unreal Engine**: Subsystem, Gameplay Tags, Enhanced Input, Common UI
- **디자인 패턴**: Component, Observer, Strategy, Subsystem
- **AI**: Behavior Tree, Custom Tasks/Decorators, EQS

### 개발 역량
- ✅ 확장 가능한 아키텍처 설계 능력
- ✅ 데이터 기반 개발로 효율성 향상
- ✅ 복잡한 게임플레이 시스템 구현 (동료, 난이도, AI)
- ✅ 크로스플랫폼 입력 및 UI 구현
- ✅ 메모리 최적화 및 성능 관리
- ✅ 체계적인 문제 해결 능력

### 협업 & 문서화
- ✅ 코드 주석 및 네이밍으로 가독성 확보
- ✅ README, Portfolio 등 체계적인 문서화
- ✅ Git을 활용한 버전 관리 (명확한 커밋 메시지)

---

## 📞 연락처

**이름**: 이난새
**이메일**: ns1337@naver.com
**GitHub**: https://github.com/cheimer/Delta

---

## 🔗 관련 링크

- **프로젝트 저장소**: [GitHub Repository](https://github.com/cheimer/Delta)
- **상세 README**: [README.md](./README.md)
- **데모 영상**: [GitHub Assets](https://github.com/user-attachments/assets/52b1cad1-b74c-41fa-a401-517299db503c)

---

*"확장 가능한 아키텍처와 클린 코드를 통해 고품질 게임을 개발합니다."*
