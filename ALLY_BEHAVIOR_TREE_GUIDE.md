# Ally Behavior Tree 구성 가이드

## Blackboard 설정

Blackboard Asset을 생성하고 다음 키들을 추가하세요:

### Required Keys
| Key Name | Type | Description |
|----------|------|-------------|
| TargetEnemy | Object (Actor) | 현재 공격 대상 적 |
| PlayerLeader | Object (Actor) | 플레이어가 조작 중인 캐릭터 |
| PatrolLocation | Vector | 순찰/대기 위치 |
| CurrentSkillRange | Float | 현재 선택된 스킬의 공격 범위 |

---

## Behavior Tree 구조 (Blueprint Editor에서 구성)

### Root Node
```
Behavior Tree Root
└─ Selector (우선순위 순서)
```

---

### Branch 1: 플레이어 복귀 (최우선)
```
Sequence "Return to Player"
├─ Decorator: CheckPlayerDistance
│  └─ Max Distance: 1500.0
│  └─ Check If Too Far: true
│
├─ Task: FollowPlayer
│  └─ Blackboard Key: PlayerLeader
│
└─ Task: MoveToPlayer
   └─ Acceptance Radius: 400.0
   └─ Speed Multiplier: 1.5
   └─ Use Urgent Speed: true
```

**설정 방법**:
1. Sequence 노드 추가 (우클릭 → Composites → Sequence)
2. Decorator 추가 (Sequence에 우클릭 → Add Decorator → CheckPlayerDistance)
   - Max Distance From Player: `1500.0`
   - Check If Too Far: `✓` (체크)
3. Task 추가: BTTask_FollowPlayer
   - Player Actor Key: `PlayerLeader` 선택
4. Task 추가: BTTask_MoveToPlayer
   - Acceptance Radius: `400.0`
   - Speed Multiplier: `1.5`
   - Use Urgent Speed: `✓`

**동작**:
- 플레이어와 1500 이상 떨어지면 이 브랜치 실행
- 빠른 속도로 플레이어에게 복귀
- 400 거리까지 접근하면 완료

---

### Branch 2: 적 공격
```
Sequence "Attack Enemy"
├─ Task: AllyTargetUpdate
│  └─ Blackboard Key: TargetEnemy
│  └─ Max Distance: 1000.0
│
├─ Decorator: Blackboard Based Condition
│  └─ Blackboard Key: TargetEnemy
│  └─ Key Query: Is Set
│
├─ Task: SetSkill
│  (스킬 선택 - 내부적으로 CurrentSkillRange 업데이트)
│
├─ Task: MoveToDynamicRadius
│  └─ Target: TargetEnemy
│  └─ Skill Range Key: CurrentSkillRange
│
└─ Task: AttackTarget
   └─ Target: TargetEnemy
```

**설정 방법**:
1. Sequence 노드 추가
2. Task 추가: BTTask_AllyTargetUpdate
   - Target Actor Key: `TargetEnemy` 선택
   - Max Distance: `1000.0`
3. Decorator 추가: Blackboard Based Condition
   - Blackboard Key: `TargetEnemy`
   - Key Query: `Is Set` 선택
4. Task 추가: BTTask_SetSkill
   (기존 enemy용 task 재활용)
5. Task 추가: BTTask_MoveToDynamicRadius
   - Target: `TargetEnemy`
   - Skill Range Key: `CurrentSkillRange`
6. Task 추가: BTTask_AttackTarget
   - Target: `TargetEnemy`

**동작**:
- 1000 범위 내 적 탐지
- 적이 있으면 스킬 선택
- 스킬 사거리로 이동
- 공격 실행

---

### Branch 3: 플레이어 추적 (기본 행동)
```
Sequence "Follow Player"
├─ Task: FollowPlayer
│  └─ Blackboard Key: PlayerLeader
│
└─ Task: MoveToPlayer
   └─ Acceptance Radius: 300.0
   └─ Speed Multiplier: 1.0
   └─ Use Urgent Speed: false
```

**설정 방법**:
1. Sequence 노드 추가
2. Task 추가: BTTask_FollowPlayer
   - Player Actor Key: `PlayerLeader`
3. Task 추가: BTTask_MoveToPlayer
   - Acceptance Radius: `300.0`
   - Speed Multiplier: `1.0`
   - Use Urgent Speed: `✗` (체크 해제)

**동작**:
- 위 두 브랜치가 모두 실패하면 실행
- 플레이어 근처(300 거리)에서 대기
- 느린 속도로 자연스럽게 추적

---

## Blueprint 생성 단계

### 1. Blackboard 생성
```
Content Browser → AI 폴더
우클릭 → Artificial Intelligence → Blackboard
이름: BB_AllyBlackboard
```

위에 설명한 4개 키 추가

### 2. Behavior Tree 생성
```
Content Browser → AI 폴더
우클릭 → Artificial Intelligence → Behavior Tree
이름: BT_AllyBehavior
```

Blackboard Asset: `BB_AllyBlackboard` 지정

### 3. 노드 배치
위에 설명한 구조대로 노드 배치

### 4. DeltaAllyController Blueprint 생성
```
Content Browser → Blueprints/AI
우클릭 → Blueprint Class → DeltaAllyController
이름: BP_AllyController
```

Class Defaults에서:
- Behavior Tree: `BT_AllyBehavior` 지정

### 5. PlayableCharacter Blueprint 설정
```
동료로 사용할 PlayableCharacter Blueprint 열기
(예: BP_Warrior, BP_Mage)

Class Defaults:
- AI Controller Class: BP_AllyController
- Auto Possess AI: Placed in World or Spawned
- Team Affiliation: Player
```

---

## 거리 기준 설명

| 상황 | 거리 | 행동 |
|------|------|------|
| 매우 멀리 떨어짐 | > 1500 | **긴급 복귀** (Branch 1) - 빠른 속도 |
| 적 발견 | < 1000 | **전투 모드** (Branch 2) - 적 공격 |
| 근처 대기 | 300-500 | **추적 모드** (Branch 3) - 플레이어 따라다님 |

---

## 동작 시나리오 예시

### 시나리오 1: 정상 상황
```
1. 플레이어 근처(400 거리)에서 시작
2. Branch 1: 거리 < 1500 → 실패
3. Branch 2: 근처에 적 없음 → 실패
4. Branch 3: 플레이어 추적 → 성공
5. 결과: 플레이어를 따라다님
```

### 시나리오 2: 적 발견
```
1. 플레이어 근처(400 거리)
2. Branch 1: 거리 < 1500 → 실패
3. Branch 2: 800 거리에 적 발견 → 성공
   - 스킬 선택
   - 적에게 이동
   - 공격
4. 적 처치 후 다시 Branch 3으로 복귀
```

### 시나리오 3: 플레이어와 멀리 떨어짐
```
1. 전투 중 플레이어가 멀리 이동
2. 거리 1800이 됨
3. Branch 1: 거리 > 1500 → 성공
   - 전투 중단
   - 빠른 속도로 플레이어에게 복귀
4. 400 거리까지 접근하면 Branch 3으로 전환
```

---

## 파라미터 튜닝 가이드

### 공격적인 AI (적극적 공격)
```
- AllyTargetUpdate Max Distance: 1500.0 (넓은 탐지)
- Return Distance: 2000.0 (멀리까지 허용)
- Follow Distance: 500.0 (멀찌감치 추적)
```

### 방어적인 AI (플레이어 근처 유지)
```
- AllyTargetUpdate Max Distance: 800.0 (좁은 탐지)
- Return Distance: 1200.0 (가까이 유지)
- Follow Distance: 200.0 (밀착 추적)
```

### 균형잡힌 AI (추천)
```
- AllyTargetUpdate Max Distance: 1000.0
- Return Distance: 1500.0
- Follow Distance: 300.0
```

---

## 문제 해결

### Ally가 움직이지 않아요
1. AI Controller가 BP_AllyController인지 확인
2. Behavior Tree가 지정되었는지 확인
3. Blackboard 키가 모두 생성되었는지 확인
4. PlayableCharacter의 Auto Possess AI 확인

### Ally가 적을 공격하지 않아요
1. Team Affiliation이 Player인지 확인
2. Skill Set Array에 스킬이 있는지 확인
3. BTTask_SetSkill이 제대로 동작하는지 확인
4. 적과의 거리 확인 (1000 이내인지)

### Ally가 플레이어를 안 따라와요
1. BTTask_FollowPlayer가 제대로 동작하는지 확인
2. Navigation Mesh가 레벨에 있는지 확인 (P 키로 확인)
3. Blackboard의 PlayerLeader 키 확인

### Ally가 너무 멀리 가요
1. CheckPlayerDistance Decorator의 Max Distance 조정
2. 1500 → 1000으로 줄여서 더 가까이 유지

---

## 고급 기능 (선택사항)

### 포메이션 시스템
Ally마다 다른 Follow Distance 설정:
- Ally 1: 300 (근접)
- Ally 2: 500 (중거리)
- Ally 3: 400 (측면)

### 역할 기반 AI
```cpp
UENUM()
enum class EAllyRole
{
    Tank,    // 앞에서 탱킹
    DPS,     // 적극적 공격
    Support  // 플레이어 근처 유지
};

// Role에 따라 다른 Behavior Tree 사용
```

### 협동 공격
```
Blackboard에 "TeamTarget" 추가
→ 여러 Ally가 같은 적 집중 공격
```

---

**작성일**: 2025-11-06
**프로젝트**: Delta
**관련 클래스**: DeltaAllyController, DeltaPlayableCharacter
