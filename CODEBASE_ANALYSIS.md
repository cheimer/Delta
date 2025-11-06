# Delta Project - Codebase Structure Analysis
## For Implementing Ally System

### PROJECT OVERVIEW
This is an Unreal Engine 5 C++ project with:
- Single playable character (ADeltaPlayableCharacter - "Back")
- Multiple enemies (ADeltaEnemyCharacter with specialized types)
- Skill-based combat system
- AI controllers with Behavior Trees
- Difficulty system

---

## 1. CHARACTER ARCHITECTURE

### Base Character Class: `ADeltaBaseCharacter`
**Location**: `/home/user/Delta/Source/Delta/Public/Characters/DeltaBaseCharacter.h`

Key Features:
- Inherits from `ACharacter` and `ISaveGameInterface`
- **Core Components**:
  - `UMotionWarpingComponent` - For character movement during skills
  - `UCombatComponent` - Handles skill execution and damage
  - `UHealthComponent` - Health/death management
  - `UManaComponent` - Skill resource system
- **Skill System**:
  - `TArray<USkillDataAsset*> SkillDataAssets` - Available skills
  - Virtual methods: `ActiveSkill()`, `PlaySkillAnimation()`, `UpdateSkillTarget()`
- **Targeting**:
  - `TWeakObjectPtr<ADeltaBaseCharacter> CurrentSkillTarget`
  - `FVector SkillTargetLocation`
  - `TArray<TEnumAsByte<EObjectTypeQuery>> TargetTraceChannel`
- **Combat**:
  - `FOnCharacterDeath` - Multicast delegate for death events
  - `TotalDealing` - Damage tracking
  - Hit detection with direction calculation

### Playable Character: `ADeltaPlayableCharacter`
**Location**: `/home/user/Delta/Source/Delta/Public/Characters/DeltaPlayableCharacter.h`

Extends `ADeltaBaseCharacter`:
- **Camera System**:
  - `USpringArmComponent` (Target Arm Length: 100-500 units)
  - `UCameraComponent` (Third-person follow)
- **Status System**:
  ```cpp
  enum class EPlayerStatus : uint8
  {
      Default,        // Normal movement
      Skill,          // Executing skill
      LockTarget,     // Targeting enemy
      WaitingSkill    // Charging/aiming skill
  };
  ```
- **Skill Management**:
  - `TArray<FSkillTypeWrapper> SkillSetArray` - Multiple skill sets (like Pokémon movesets)
  - Each set contains 3 skill slots: `TArray<EDeltaSkillType>`
  - `CurrentSkillSetIndex` - Active skill set index
  - Navigation: `SkillBefore()`, `SkillNext()`
- **Target Locking**:
  - `FindEnemyFromFront()` - Detects enemies in front
  - `UpdateLockTarget()` - Updates locked target
  - Lock-on camera rotation during combat
- **Input System**: Enhanced Input System (UE5 standard)
  - Discrete inputs for each skill slot (Skill1, Skill2, Skill3)
  - Scroll for changing skill sets
  - Lock target toggle
  - Parrying, Execute buttons

### Enemy Character: `ADeltaEnemyCharacter`
**Location**: `/home/user/Delta/Source/Delta/Public/Characters/Enemy/DeltaEnemyCharacter.h`

Extends `ADeltaBaseCharacter`:
- **AI Controller**: `ADeltaAIController` (possessed after spawn)
- **Skill Selection**: Random skill from `SkillDataAssets`
  - `SetCurrentSkill()` - Picks random skill
  - `GetCurrentSkill()` - Returns current skill type
- **Difficulty Scaling**: `ApplyDifficultySettings()`
  ```cpp
  Easy:   Health x0.75,  Damage x0.5
  Normal: Health x1.0,   Damage x1.0
  Hard:   Health x1.5,   Damage x1.5
  ```

### Special Enemy: `ADesertDragonCharacter`
**Location**: `/home/user/Delta/Source/Delta/Public/Characters/Enemy/DesertDragonCharacter.h`
- Boss enemy with special mechanics
- Flying state handling
- Unique skill set

---

## 2. COMBAT SYSTEM

### Combat Component: `UCombatComponent`
**Location**: `/home/user/Delta/Source/Delta/Public/Components/CombatComponent.h`

Key Responsibilities:
- **Skill Lifecycle**:
  - `BeginSkill(SkillClass)` - Creates skill instance
  - `EndSkill()` - Cleans up skill
  - `ReleaseSkill()` - Fully removes skill
  - `CancelSkill(BlendOutTime)` - Interrupts with montage blend-out
- **Damage Application**:
  - `ApplySkillDamage(Target, Causer, Damage)` - Applies skill damage with multipliers
  - `TakeDamage()` - React to incoming damage
- **Multipliers**:
  - `DamageMultiplier` - Outgoing damage scaling (difficulty)
  - `DamageTakenMultiplier` - Incoming damage scaling
- **Target Detection**:
  - `GetIsOpponent()` - Checks if actor is valid target
  - `GetSkillTargetActor()` - Gets current target
  - `GetSkillTargetLocation()` - Gets aim point

### Health Component: `UHealthComponent`
**Location**: `/home/user/Delta/Source/Delta/Public/Components/HealthComponent.h`

Features:
- `MaxHealth` and `CurrentHealth` tracking
- `TakeDamage(Damage, Causer)` - Processes damage
- `FOnHealthChanged` - Broadcasts health updates (UI)
- Death state flag
- Percentage-based health getters/setters

### Skill System: `USkillBase`
**Location**: `/home/user/Delta/Source/Delta/Public/Skills/SkillBase.h`

Architecture:
- Abstract base class for all skills
- `EDeltaSkillType` enum (26 total skill types)
- Virtual methods:
  - `BeginSkill()` - Initialize skill
  - `ActInput()` - Handle player input during skill
  - `ReactDamaged()` - React to interruption
  - `EndSkill()` - Cleanup
- Skill data stored in `USkillDataAsset`

### Skill Types (EDeltaSkillType)
**Playable Character Skills**:
- Parrying, Shield, MeteorShower, Slash, Lightning, GroundCrack
- Flashes, ArrowShot, FlyingDisk, RailGun

**Desert Dragon Skills**:
- LeftClaws, RightClaws, DoubleClaws, Bite, SpitFire, SpreadFire
- FlySpitFire, FlySpreadFire

**Wood Mon Skills**:
- Swing, BlowAway, ThrowRock, TreeBloom, ForestBoom

---

## 3. AI SYSTEM

### AI Controller: `ADeltaAIController`
**Location**: `/home/user/Delta/Source/Delta/Public/Controllers/DeltaAIController.h`

Key Features:
- Inherits from `AAIController`
- **Behavior Tree**: Runs assigned BT on possess
- **Target Management**:
  - `GetRandPlayableTarget(MaxDistance = 1000)` - Finds random playable target in range
  - `CurrentPlayableTarget` - Weak reference to target
  - `PlayableTargets` - Array of all playable characters
- **Skill Control**:
  - `SetCurrentSkill()` - Commands enemy to pick skill
  - `GetCurrentSkillRange()` - Queries skill range
  - `GetCurrentSkillDuration()` - Queries animation duration
  - `AttackTarget()` - Plays skill animation
- **Navigation**:
  - `SpawnLocation` - Starting position (for return behavior)

### Behavior Tree Tasks
Located in `/home/user/Delta/Source/Delta/Public/AI/`:

1. **`UBTTask_TargetUpdate`**: Updates target from playable characters
   - Uses blackboard key selector
   - Max distance parameter (default 1000)

2. **`UBTTask_SetSkill`**: Calls controller's SetCurrentSkill()

3. **`UBTTask_AttackTarget`**: Executes attack animation

4. **`UBTTask_MoveToLocation`**: Moves to target/location

5. **`UBTTask_MoveToDynamicRadius`**: Dynamic movement based on skill range

6. **`UBTTask_Heal`**: Self-healing behavior

7. **`UBTTask_UpdateSpawnLocation`**: Records spawn point

8. **`UBTTask_WaitAttackDelay`**: Waits between attacks

9. **`UBTDecorator_IsPlayableClose`**: Checks if playable target in range

10. **`UBTDecorator_IsAtSpawnLocation`**: Checks if at spawn point

11. **`UBTDecorator_IsFlying`**: Checks flying state

---

## 4. PLAYER CONTROLLER & INPUT

### Player Controller: `ADeltaPlayerController`
**Location**: `/home/user/Delta/Source/Delta/Public/Controllers/DeltaPlayerController.h`

Responsibilities:
- Manages player character (weak reference)
- **UI Communication**:
  - `TargetDetected()` / `TargetLost()` - Skill target detection
  - `GetSkillTextures()` / `GetSkillCosts()` - UI data
  - `SetHudVisible()` - UI visibility control
- **Game State**:
  - `GameEnd()` - End game
  - `IsPlayerWin()` - Victory check
  - `GetPlayingTime()`, `GetTotalDamage()`, `GetPlayerHealth()`
- **Input Control**:
  - `SetInputEnable(bool)` - Freeze/unfreeze input
  - `bIsInputEnable` flag
- **Save System**: Implements `ISaveGameInterface`

### Input System
**UInputDataAsset**:
- Maps input actions to character functions
- `InputActionConfigs` array:
  ```cpp
  {InputActionPtr, TriggerEvent, FunctionName}
  ```
- Used by both `ADeltaPlayableCharacter` and `ADeltaPlayerController`

**Playable Character Input Functions**:
```cpp
- Move(InputActionValue)      // WASD movement
- Look(InputActionValue)       // Mouse look
- Scroll(InputActionValue)     // Zoom in/out
- LockTarget(InputActionValue) // L to lock enemy
- Execute(InputActionValue)    // Confirm/execute skill
- Parrying(InputActionValue)   // Parry defense
- SkillFirst/Second/Third()    // Skill slots 1-3
- SkillBefore/Next()           // Change skill set
```

---

## 5. GAME MODE & LEVEL MANAGEMENT

### Game Mode: `ADeltaBaseGameMode`
**Location**: `/home/user/Delta/Source/Delta/Public/GameModes/DeltaBaseGameMode.h`

Architecture:
```cpp
enum class EGameModeState
{
    WaitForStart,
    Playing,
    Result
};
```

Key Features:
- **Character Collection** (in BeginPlay):
  - `TArray<TWeakObjectPtr<ADeltaBaseCharacter>> PlayableCharacters`
  - `TArray<TWeakObjectPtr<ADeltaBaseCharacter>> EnemyCharacters`
  - Collects all DeltaPlayableCharacter and DeltaEnemyCharacter from level
  - Subscribes to death events
- **State Management**:
  - `GameStart()` - Initializes game, triggers save load
  - `GameEnd()` - Calls player controller's GameEnd()
  - `IsPlayerWin()` - Checks if all enemies defeated
- **Time Tracking**:
  - `GetPlayingTime()` - Elapsed time
  - `GameStartTime` - When game began

---

## 6. TYPE DEFINITIONS

### Skill Enums: `EDeltaEnumTypes.h`

**Skill Type** (EDeltaSkillType):
- 26 unique skill types (see section 2)

**Skill State** (EDeltaSkillState):
- Back (playable character state)
- DesertDragon_Ground, DesertDragon_Fly
- WoodMon

**Player Status** (EPlayerStatus):
- Default, Skill, LockTarget, WaitingSkill

**Difficulty** (EDeltaGameDifficulty):
- Easy, Normal, Hard

**Hit Direction** (EDeltaHitDirection):
- Anywhere, Forward, Backward, Left, Right

---

## 7. KEY SYSTEMS OVERVIEW

```
┌─────────────────────────────────────────────────────────┐
│              PLAYER SYSTEM                              │
├─────────────────────────────────────────────────────────┤
│ ADeltaPlayableCharacter                                 │
│ ├─ DeltaPlayerController (possessed controller)         │
│ ├─ Camera System (SpringArm + Camera)                   │
│ ├─ Input System (Enhanced Input)                        │
│ ├─ Skill Sets (3 skills per set, multiple sets)         │
│ ├─ Lock-Target System                                   │
│ └─ Status Machine (Default/Skill/LockTarget/Waiting)    │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│              ENEMY SYSTEM                               │
├─────────────────────────────────────────────────────────┤
│ ADeltaEnemyCharacter                                    │
│ ├─ DeltaAIController (AI decision making)               │
│ ├─ Behavior Tree (decision tree)                        │
│ ├─ Random Skill Selection                               │
│ ├─ Difficulty Scaling                                   │
│ └─ Target from PlayableTargets array                    │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│              COMBAT SYSTEM (SHARED)                     │
├─────────────────────────────────────────────────────────┤
│ Both inherit from ADeltaBaseCharacter                    │
│ ├─ CombatComponent (skill lifecycle & damage)           │
│ ├─ HealthComponent (health tracking)                    │
│ ├─ ManaComponent (resource)                             │
│ ├─ MotionWarpingComponent (position during skills)      │
│ └─ Animation System (montages for skills)               │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│              GAME MANAGEMENT                            │
├─────────────────────────────────────────────────────────┤
│ ADeltaBaseGameMode                                      │
│ ├─ Collects all characters in level                     │
│ ├─ Manages game state transitions                       │
│ ├─ Tracks playable vs enemy characters                  │
│ ├─ Handles death events                                 │
│ └─ Determines victory conditions                        │
└─────────────────────────────────────────────────────────┘
```

---

## 8. CURRENT LIMITATIONS FOR ALLY SYSTEM

**What Exists**:
- Single playable character control (DeltaPlayableCharacter)
- Multiple enemy management (game mode collects enemies)
- Flexible skill system (usable by any character)
- AI controller system (reusable for allies)
- Damage/health systems (generic)

**What's Missing**:
- **No ally characters** - Currently only ADeltaPlayableCharacter and ADeltaEnemyCharacter
- **No multi-character input** - Controller only possesses one character
- **No team management** - Game mode tracks characters but doesn't manage teams
- **No character switching** - No input/UI for switching between allies
- **No ally AI** - Enemies use AI, but there's no friendly AI controller
- **No ally targeting** - Targeting only works for enemies
- **No group tactics** - No formation, coordinated attacks, etc.

---

## 9. KEY FILE LOCATIONS

### Character Files
```
Characters/
├── DeltaBaseCharacter.h/cpp         (Base class)
├── DeltaPlayableCharacter.h/cpp     (Player character)
└── Enemy/
    ├── DeltaEnemyCharacter.h/cpp    (Enemy base)
    └── DesertDragonCharacter.h/cpp  (Boss enemy)
```

### Controller Files
```
Controllers/
├── DeltaPlayerController.h/cpp      (Player input/state)
├── DeltaAIController.h/cpp          (Enemy AI)
└── DeltaFrontPlayerController.h/cpp (Menu controller)
```

### Component Files
```
Components/
├── CombatComponent.h/cpp            (Skill execution)
├── HealthComponent.h/cpp            (Health/death)
├── ManaComponent.h/cpp              (Resource)
├── PhaseComponent.h/cpp             (Boss phases)
└── CutSceneComponent.h/cpp          (Cinematic)
```

### AI Files
```
AI/
├── BTTask_TargetUpdate.h/cpp
├── BTTask_SetSkill.h/cpp
├── BTTask_AttackTarget.h/cpp
├── BTTask_MoveToLocation.h/cpp
├── BTTask_MoveToDynamicRadius.h/cpp
├── BTTask_Heal.h/cpp
├── BTTask_UpdateSpawnLocation.h/cpp
├── BTTask_WaitAttackDelay.h/cpp
├── BTDecorator_IsPlayableClose.h/cpp
├── BTDecorator_IsAtSpawnLocation.h/cpp
└── BTDecorator_IsFlying.h/cpp
```

### Skill/Combat Files
```
Skills/
├── SkillBase.h/cpp                  (Base skill class)
├── SkillProjectile.h/cpp            (Projectile skill)
├── SkillCollisionAttack.h/cpp       (Melee skill)
├── SkillParrying.h/cpp              (Defense)
├── SkillShield.h/cpp                (Defense)
├── [26 total skill implementations]
└── Projectiles/
    ├── ProjectileBase.h/cpp
    ├── ProjectileFlyingDisk.h/cpp
    ├── ProjectileSpreadFire.h/cpp
    └── ProjectileSpitFire.h/cpp
```

### Type Definitions
```
DeltaTypes/
├── DeltaEnumTypes.h                 (All enums)
├── DeltaStructTypes.h               (All structs)
└── DeltaNamespaceTypes.h            (Namespaces)
```

---

## 10. IMPLEMENTATION RECOMMENDATIONS FOR ALLY SYSTEM

### Phase 1: Foundation
1. Create `ADeltaAllyCharacter` - Inherits from `ADeltaBaseCharacter`
2. Create `ADeltaAllyController` - AI controller for allies (similar to ADeltaAIController but with friendly team)
3. Modify `ADeltaBaseGameMode` to track allies separately
4. Extend targeting system to understand teams (allies vs enemies)

### Phase 2: Character Control
1. Add team/party management to player controller
2. Implement character switching input
3. Create UI for team management
4. Handle possession/unpossession of different allies

### Phase 3: AI & Coordination
1. Implement ally AI controller with different behavior strategies
2. Add formation system
3. Implement command system (attack, defend, heal, etc.)
4. Add coordinated ability system

### Phase 4: Polish
1. Add team-based damage scaling
2. Implement shared resources (team mana/energy)
3. Add team combo systems
4. Save/load team states

