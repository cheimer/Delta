# 동료 시스템 (Ally System) 사용 가이드

## 개요

Delta 프로젝트에 동료 시스템이 구현되었습니다. 이 시스템을 통해:
- AI 동료가 플레이어와 함께 적을 공격합니다
- 플레이어가 팀원 간 전환하여 직접 조작할 수 있습니다
- 팀 기반 전투 시스템을 활용할 수 있습니다

## 주요 기능

### 1. 팀 소속 시스템 (Team Affiliation)
- **ETeamAffiliation** enum이 추가되었습니다:
  - `Player`: 플레이어 팀
  - `Enemy`: 적 팀
  - `Neutral`: 중립 (모두에게 적대적)

### 2. 동료 캐릭터 (Ally Character)
- `ADeltaAllyCharacter` 클래스가 생성되었습니다
- 위치: `Source/Delta/Public/Characters/DeltaAllyCharacter.h`
- 기능:
  - AI 컨트롤러로 자동 전투
  - 플레이어가 직접 조작 가능
  - 자동으로 적을 탐지하고 공격

### 3. 캐릭터 전환 시스템
`ADeltaPlayerController`에 팀원 관리 기능이 추가되었습니다:

```cpp
// 팀원 등록
void RegisterTeamMember(ADeltaBaseCharacter* TeamMember);

// 다음 캐릭터로 전환
void SwitchToNextCharacter();

// 이전 캐릭터로 전환
void SwitchToPreviousCharacter();

// 특정 인덱스 캐릭터로 전환
void SwitchToCharacterByIndex(int32 Index);
```

### 4. AI 동료 컨트롤러
- `ADeltaAllyController` 클래스가 생성되었습니다
- 위치: `Source/Delta/Public/Controllers/DeltaAllyController.h`
- 기능:
  - 적 탐지 및 공격
  - 플레이어 추적
  - 자동 스킬 사용

### 5. Behavior Tree 태스크
동료 AI를 위한 새로운 태스크가 추가되었습니다:

- **BTTask_AllyTargetUpdate**: 적 타겟 탐지
- **BTTask_FollowPlayer**: 플레이어 추적

## 사용 방법

### Blueprint에서 동료 캐릭터 생성하기

1. **Blueprint 생성**
   - Content Browser에서 우클릭
   - Blueprint Class → `DeltaAllyCharacter` 선택
   - 이름 지정 (예: `BP_AllyWarrior`)

2. **캐릭터 설정**
   - 메시, 애니메이션 설정
   - Team Affiliation이 `Player`로 설정되어 있는지 확인
   - Display Name 설정 (동료 이름)

3. **AI Controller 설정**
   - Blueprint를 열고 Class Defaults 선택
   - AI Controller Class를 `DeltaAllyController`로 설정
   - Auto Possess AI를 `Placed in World or Spawned`로 설정

4. **Behavior Tree 생성**
   - Content Browser에서 우클릭
   - Artificial Intelligence → Behavior Tree
   - Blackboard도 함께 생성

5. **Behavior Tree 구성**
   기본 구조 예시:
   ```
   Root
   └─ Selector
      ├─ Sequence: "공격"
      │  ├─ BTTask_AllyTargetUpdate (적 찾기)
      │  ├─ BTTask_MoveToDynamicRadius (적에게 이동)
      │  └─ BTTask_AttackTarget (공격)
      └─ Sequence: "추적"
         ├─ BTTask_FollowPlayer (플레이어 찾기)
         └─ BTTask_MoveToDynamicRadius (플레이어에게 이동)
   ```

6. **레벨에 배치**
   - 생성한 동료 Blueprint를 레벨에 드래그
   - 게임 시작 시 자동으로 GameMode에 등록됩니다

### C++에서 동료 생성하기

```cpp
// 동료 스폰 예제
ADeltaBaseGameMode* GameMode = Cast<ADeltaBaseGameMode>(GetWorld()->GetAuthGameMode());
if (GameMode)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ADeltaAllyCharacter* NewAlly = GetWorld()->SpawnActor<ADeltaAllyCharacter>(
        AllyClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (NewAlly)
    {
        // GameMode에 등록
        GameMode->RegisterAlly(NewAlly);
    }
}
```

### 캐릭터 전환 입력 설정

1. **Input Action 생성**
   - Content Browser에서 Input → Input Action 생성
   - 이름: `IA_SwitchCharacter`

2. **Input Mapping Context에 추가**
   - 기존 Input Mapping Context 열기
   - 새 매핑 추가:
     - Action: `IA_SwitchCharacter`
     - Key: Tab 또는 원하는 키

3. **PlayerController에서 바인딩**
   ```cpp
   // DeltaPlayerController의 SetupInputComponent에 추가
   EnhancedInputComponent->BindAction(
       SwitchCharacterAction,
       ETriggerEvent::Triggered,
       this,
       &ADeltaPlayerController::SwitchToNextCharacter
   );
   ```

## 스킬 시스템 설정

동료 캐릭터도 기존 스킬 시스템을 사용할 수 있습니다:

1. **Skill Data Asset 설정**
   - 동료 Blueprint의 Class Defaults에서
   - Skill Data Assets 배열에 사용할 스킬 추가

2. **스킬 세트 구성**
   - 플레이어와 동일한 방식으로 스킬 설정
   - 각 스킬의 범위, 데미지, 애니메이션 지정

## 게임 모드 설정

`ADeltaBaseGameMode`가 자동으로 동료를 추적합니다:

- 모든 플레이어 팀원(플레이어 + 동료)이 죽으면 패배
- 모든 적이 죽으면 승리
- 동료가 죽어도 다른 팀원이 살아있으면 게임 계속

## 팀 기반 전투 시스템

### 타게팅 로직
`CombatComponent`의 `GetIsOpponent()` 함수가 팀 소속을 확인합니다:

```cpp
// 같은 팀끼리는 공격 불가
if (OwnerTeam == CheckTeam) return false;

// 다른 팀은 적대적
if (OwnerTeam != CheckTeam) return true;

// Neutral은 모두에게 적대적
if (OwnerTeam == Neutral || CheckTeam == Neutral) return true;
```

### AI 타게팅
- **적 AI**: `GetRandPlayableTarget()` - 플레이어 팀을 찾음
- **동료 AI**: `GetRandEnemyTarget()` - 적 팀을 찾음

## 파일 구조

### 새로 추가된 파일
```
Source/Delta/
├── Public/
│   ├── Characters/
│   │   └── DeltaAllyCharacter.h
│   ├── Controllers/
│   │   └── DeltaAllyController.h
│   ├── AI/
│   │   ├── BTTask_AllyTargetUpdate.h
│   │   └── BTTask_FollowPlayer.h
│   └── DeltaTypes/
│       └── DeltaEnumTypes.h (ETeamAffiliation 추가)
└── Private/
    ├── Characters/
    │   └── DeltaAllyCharacter.cpp
    ├── Controllers/
    │   └── DeltaAllyController.cpp
    └── AI/
        ├── BTTask_AllyTargetUpdate.cpp
        └── BTTask_FollowPlayer.cpp
```

### 수정된 파일
```
Source/Delta/
├── Public/
│   ├── Characters/
│   │   └── DeltaBaseCharacter.h (TeamAffiliation 추가)
│   ├── Controllers/
│   │   └── DeltaPlayerController.h (팀 관리 기능 추가)
│   └── GameModes/
│       └── DeltaBaseGameMode.h (동료 추적 기능 추가)
└── Private/
    ├── Characters/
    │   ├── DeltaPlayableCharacter.cpp (TeamAffiliation = Player)
    │   └── Enemy/DeltaEnemyCharacter.cpp (TeamAffiliation = Enemy)
    ├── Components/
    │   └── CombatComponent.cpp (팀 기반 타게팅)
    ├── Controllers/
    │   └── DeltaPlayerController.cpp (캐릭터 전환 구현)
    └── GameModes/
        └── DeltaBaseGameMode.cpp (동료 관리 구현)
```

## 주의사항

1. **Behavior Tree 필수**
   - 동료 캐릭터는 반드시 Behavior Tree와 Blackboard가 필요합니다
   - AI Controller에 Behavior Tree를 지정하지 않으면 작동하지 않습니다

2. **팀 소속 설정**
   - 캐릭터의 Team Affiliation을 올바르게 설정해야 합니다
   - 잘못 설정하면 아군끼리 공격할 수 있습니다

3. **스킬 데이터**
   - 동료도 스킬을 사용하려면 Skill Data Assets를 설정해야 합니다
   - Enemy Character의 스킬 시스템을 참고하세요

4. **캐릭터 전환**
   - 죽은 캐릭터로는 전환할 수 없습니다
   - 전환 시 카메라가 자동으로 따라갑니다

## 예제 시나리오

### 시나리오 1: 2명의 동료와 함께 전투
1. 레벨에 동료 2명 배치 (BP_AllyWarrior, BP_AllyMage)
2. 게임 시작 시 자동으로 플레이어 컨트롤러에 등록
3. Tab 키로 3명의 캐릭터 간 전환
4. AI 동료는 자동으로 적을 공격하고 플레이어를 따라다님

### 시나리오 2: 런타임에 동료 추가
```cpp
// 특정 이벤트 발생 시 동료 추가
void AMyGameMode::SpawnAllyAtCheckpoint(FVector Location)
{
    ADeltaAllyCharacter* NewAlly = GetWorld()->SpawnActor<ADeltaAllyCharacter>(
        AllyWarriorClass,
        Location,
        FRotator::ZeroRotator
    );

    if (NewAlly)
    {
        RegisterAlly(NewAlly);
        UE_LOG(LogTemp, Log, TEXT("새로운 동료가 합류했습니다!"));
    }
}
```

## 문제 해결

### 동료가 움직이지 않아요
- AI Controller가 올바르게 설정되었는지 확인
- Behavior Tree가 실행 중인지 확인 (에디터에서 디버깅 가능)
- Auto Possess AI 설정 확인

### 동료가 적을 공격하지 않아요
- Team Affiliation이 올바르게 설정되었는지 확인
- Skill Data Assets가 추가되었는지 확인
- BTTask_AllyTargetUpdate의 MaxDistance 값 확인

### 캐릭터 전환이 안 돼요
- Input Action이 올바르게 바인딩되었는지 확인
- TeamMembers 배열에 캐릭터가 등록되었는지 확인
- 플레이어 컨트롤러에서 GetTeamMembers() 함수로 확인 가능

## 향후 개선 사항

1. **명령 시스템**: 동료에게 특정 명령 내리기 (공격, 방어, 대기 등)
2. **포메이션**: 동료들의 대형 설정
3. **팀 콤보**: 특정 조합으로 강력한 스킬 사용
4. **동료 UI**: 각 동료의 체력, 스킬 쿨다운 표시
5. **동료 업그레이드**: 동료의 능력치 향상 시스템

## 참고 문서

- CODEBASE_ANALYSIS.md: 전체 코드베이스 구조
- EXPLORATION_SUMMARY.md: 시스템 개요
- DOCUMENTATION_INDEX.md: 문서 인덱스

---

**작성일**: 2025-11-05
**버전**: 1.0
**브랜치**: claude/implement-ally-system-011CUqeVLyGHT8vESD4gkxKj
