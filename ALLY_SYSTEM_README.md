# 동료 시스템 사용 가이드 v2.0

## 핵심 개념

**통합 캐릭터 시스템**: 모든 플레이어 팀 캐릭터가 `DeltaPlayableCharacter`를 사용합니다.

- **한 클래스, 두 모드**: Player Controller 또는 AI Controller로 제어
- **동적 전환**: 언제든지 플레이어 <-> AI 모드 전환 가능
- **동일한 기능**: 모든 캐릭터가 스킬, 전투, 애니메이션 시스템 공유

## 빠른 시작

### 1. 동료 캐릭터 만들기 (Blueprint)

```
1. Content Browser → Blueprint Class → DeltaPlayableCharacter
2. 이름: BP_Warrior, BP_Mage 등
3. 설정:
   - Mesh, Animation
   - Team Affiliation = Player
   - Skill Set Array에 스킬 추가
   - AI Controller Class = DeltaAllyController
   - Auto Possess AI = Placed in World or Spawned
```

### 2. Behavior Tree 설정

```
기본 구조:
Root
└─ Selector
   ├─ 공격: AllyTargetUpdate → SetSkill → Move → Attack
   └─ 추적: FollowPlayer → Move
```

### 3. 레벨 배치

```
- 여러 PlayableCharacter를 레벨에 배치
- 첫 번째는 플레이어가 시작
- 나머지는 AI가 자동 컨트롤
```

### 4. 캐릭터 전환 입력

```cpp
// Input Action: IA_SwitchCharacter
// Key: Tab

// 바인딩:
PlayerController->SwitchToNextCharacter();
```

## 주요 기능

### DeltaPlayableCharacter

```cpp
// AI 지원
void SetRandomSkill();                    // AI가 스킬 선택
EDeltaSkillType GetCurrentSkill();        // 현재 선택된 스킬
void OnPlayerControlStart();              // Player 모드 시작
void OnPlayerControlEnd();                // Player 모드 종료
bool IsPlayerControlled();                // 현재 모드 확인
```

### DeltaAllyController

```cpp
// AI Controller for PlayableCharacter
AActor* GetRandEnemyTarget();             // 적 탐지
ADeltaPlayableCharacter* GetPlayerLeader(); // 플레이어 찾기
void SetCurrentSkill();                    // 스킬 설정
void AttackTarget();                       // 공격 실행
```

### DeltaPlayerController

```cpp
// 팀 관리
void RegisterTeamMember(Character*);       // 팀원 등록
void SwitchToNextCharacter();              // 다음으로 전환
void SwitchToPreviousCharacter();          // 이전으로 전환
void SwitchToCharacterByIndex(int);        // 특정 캐릭터로
```

### DeltaBaseGameMode

```cpp
// 팀원 관리
void RegisterTeamMember(PlayableCharacter*);
```

## 런타임 팀원 추가

```cpp
// 새 팀원 스폰
ADeltaPlayableCharacter* NewMember = GetWorld()->SpawnActor<ADeltaPlayableCharacter>(
    MemberClass, Location, Rotation
);

// 게임 모드에 등록 (자동으로 플레이어 컨트롤러에도 등록됨)
GameMode->RegisterTeamMember(NewMember);
```

## 팀 소속 시스템

```cpp
enum class ETeamAffiliation
{
    Player,   // 플레이어 팀 (모든 PlayableCharacter)
    Enemy,    // 적 팀 (EnemyCharacter)
    Neutral   // 중립 (모두에게 적대)
};

// 전투 시스템이 자동으로 팀을 확인하여 공격 가능 여부 판단
```

## 예제 시나리오

### 3인 팀 전투

```
레벨:
- BP_Knight (Player 시작)
- BP_Mage (AI)
- BP_Archer (AI)

게임플레이:
1. Knight로 시작
2. Mage, Archer는 AI가 자동 전투
3. Tab → Mage로 전환하여 직접 조작
4. Tab → Archer로 전환
5. Tab → 다시 Knight로
```

## 문제 해결

**동료가 움직이지 않아요**
- AI Controller = DeltaAllyController 확인
- Behavior Tree 지정 확인
- Auto Possess AI 설정 확인

**동료가 공격하지 않아요**
- Team Affiliation = Player 확인
- Skill Set Array에 스킬 있는지 확인
- BTTask_SetSkill 사용 확인

**캐릭터 전환 안 돼요**
- Input Action 바인딩 확인
- TeamMembers 배열에 등록되었는지 확인
- 캐릭터가 살아있는지 확인

## 기술 세부사항

### 모드 전환 메커니즘

```cpp
// Player 모드
- PlayerController가 Possess
- Input 활성화
- Tick에서 카메라 업데이트
- UI 표시

// AI 모드
- AIController가 Possess
- Behavior Tree 실행
- SetRandomSkill()로 스킬 선택
- 자동 전투
```

### 승패 조건

```
승리: 모든 적 제거
패배: 모든 팀원 (PlayableCharacter) 사망
```

## 파일 구조

```
수정된 파일:
- DeltaPlayableCharacter.h/cpp (AI 모드 지원 추가)
- DeltaAllyController.h/cpp (PlayableCharacter 사용)
- DeltaBaseGameMode.h/cpp (통합 관리)
- DeltaPlayerController.h/cpp (PlayableCharacter 전환)
- DeltaEnumTypes.h (ETeamAffiliation)
- DeltaBaseCharacter.h (TeamAffiliation 속성)
- CombatComponent.cpp (팀 기반 타게팅)

새 파일:
- BTTask_AllyTargetUpdate.h/cpp
- BTTask_FollowPlayer.h/cpp

제거된 개념:
- DeltaAllyCharacter (PlayableCharacter로 통합)
```

## 설계 장점

1. **간단함**: 하나의 클래스만 사용
2. **유연함**: 모든 캐릭터를 Player/AI로 전환 가능
3. **확장성**: 새 캐릭터 = 새 PlayableCharacter Blueprint
4. **일관성**: 모든 캐릭터가 동일한 기능 사용

---

**버전**: 2.0 (Unified Character System)
**날짜**: 2025-11-06
**브랜치**: claude/implement-ally-system-011CUqeVLyGHT8vESD4gkxKj

## 변경 이력

**v2.0**: Ally 전용 클래스 제거, PlayableCharacter로 통합
**v1.0**: 초기 버전 (별도 AllyCharacter 사용)
