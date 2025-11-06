# Delta Project - Detailed Code Reference
## Implementation Details & Code Examples

---

## CLASS HIERARCHY

```
ACharacter (Unreal Engine)
    └─ ADeltaBaseCharacter
        ├─ ADeltaPlayableCharacter
        └─ ADeltaEnemyCharacter
            └─ ADesertDragonCharacter (Special Boss)

AAIController (Unreal Engine)
    └─ ADeltaAIController

APlayerController (Unreal Engine)
    ├─ ADeltaPlayerController
    └─ ADeltaFrontPlayerController

UActorComponent (Unreal Engine)
    ├─ UCombatComponent
    ├─ UHealthComponent
    ├─ UManaComponent
    ├─ UPhaseComponent
    ├─ UCutSceneComponent
    └─ UMotionWarpingComponent (Plugin)

UObject
    ├─ USkillBase (Abstract)
    │   ├─ USkillProjectile
    │   ├─ USkillCollisionAttack
    │   ├─ USkillParrying
    │   ├─ USkillShield
    │   ├─ [... 21 other skill implementations]
    │   └─ Projectiles/
    │       ├─ AProjectileBase
    │       ├─ AProjectileFlyingDisk
    │       ├─ AProjectileSpreadFire
    │       └─ AProjectileSpitFire
    │
    └─ UDataAsset
        ├─ USkillDataAsset
        └─ UInputDataAsset

UBTTaskNode
    ├─ UBTTask_TargetUpdate
    ├─ UBTTask_SetSkill
    ├─ UBTTask_AttackTarget
    ├─ UBTTask_MoveToLocation
    ├─ UBTTask_MoveToDynamicRadius
    ├─ UBTTask_Heal
    ├─ UBTTask_UpdateSpawnLocation
    └─ UBTTask_WaitAttackDelay

UBTDecorator
    ├─ UBTDecorator_IsPlayableClose
    ├─ UBTDecorator_IsAtSpawnLocation
    └─ UBTDecorator_IsFlying
```

---

## DETAILED FLOW EXAMPLES

### 1. PLAYABLE CHARACTER SKILL EXECUTION FLOW

```
User Input (Skill1)
    ↓
ADeltaPlayableCharacter::SkillFirst()
    ├─ Validates: Status == Default or LockTarget
    ├─ Validates: Not dead
    ├─ Validates: Skill exists in SkillSetArray[CurrentSkillSetIndex]
    └─ StartWaitingSkill(KeyIndex)
        ├─ Validates: Mana available
        ├─ Sets CurrentStatus = WaitingSkill
        ├─ Caches skill data
        └─ OnSelectSkill.Broadcast() → Updates UI
            ↓
    Tick: UpdateSkillTarget()
        ├─ Traces from camera to find target
        ├─ Updates SkillTargetLocation
        └─ Broadcasts target to PlayerController
            ↓
    User Confirms Input
        ↓
    PlaySkillAnimation()
        ├─ Sets CurrentStatus = Skill
        ├─ Plays montage
        ├─ Creates motion warping target
        └─ AnimNotify triggers in montage:
            ├─ AnimNotify_ActiveSkill
            │   ├─ Calls CombatComponent->BeginSkill(SkillClass)
            │   ├─ Creates USkillBase instance
            │   └─ Calls BeginSkill() on skill object
            │       ├─ Skills initialize collision volumes
            │       └─ Skills set up projectiles/effects
            │
            └─ AnimNotifyState_ActiveSkill
                ├─ During skill active window:
                ├─ Checks for hits on collision volumes
                ├─ Applies damage via CombatComponent->ApplySkillDamage()
                └─ Knockback application
                    ↓
    Montage Completes
        ├─ EndSkillAnimation()
        ├─ Sets CurrentStatus = Default
        ├─ CombatComponent->EndSkill()
        ├─ CombatComponent->ReleaseSkill()
        └─ Ready for next input
```

### 2. ENEMY CHARACTER AI EXECUTION FLOW

```
ADeltaEnemyCharacter spawned in level
    ↓
BeginPlay()
    ├─ ApplyDifficultySettings()
    │   └─ Modifies Health and Damage multipliers
    └─ Super::BeginPlay()
        ↓
PossessedBy(ADeltaAIController)
    ├─ Controller runs Behavior Tree
    ├─ BT executes tasks in sequence
    │
    ├─ BTTask_TargetUpdate
    │   └─ Calls Controller->GetRandPlayableTarget()
    │       ├─ Scans for ADeltaPlayableCharacter in level
    │       ├─ Filters by MaxDistance (1000 units)
    │       └─ Sets blackboard TargetActorKey
    │
    ├─ BTDecorator_IsPlayableClose
    │   └─ Checks if target within range
    │
    ├─ BTTask_MoveToLocation (or MoveToDynamicRadius)
    │   └─ Uses Unreal NavMesh to move toward target
    │
    ├─ BTTask_SetSkill
    │   └─ Calls Enemy->SetCurrentSkill()
    │       └─ Random skill from SkillDataAssets array
    │
    ├─ BTTask_WaitAttackDelay
    │   └─ Waits X seconds (from multiplier)
    │
    └─ BTTask_AttackTarget
        └─ Calls Controller->AttackTarget()
            └─ Enemy->PlaySkillAnimation(CurrentSkill)
                ├─ Updates skill target (finds CurrentPlayableTarget)
                ├─ Plays montage
                └─ Follows same animation notify flow as player
                    ↓
    Loop: Behavior tree repeats from TargetUpdate
```

### 3. DAMAGE FLOW

```
Skill Collision Hit Detected
    ↓
AnimNotifyState_ActiveSkill processing
    ├─ HitActors array populated from overlap checks
    └─ For each hit actor:
        ↓
    CombatComponent->ApplySkillDamage(HitActor, Causer, SkillDamage)
        ├─ Applies multiplier: SkillDamage * DamageMultiplier
        └─ UGameplayStatics::ApplyDamage()
            ↓
    ADeltaBaseCharacter::TakeSkillDamage() (Damage callback)
        ├─ Calls HealthComponent->TakeDamage()
        │   ├─ CurrentHealth -= Damage * DamageTakenMultiplier
        │   ├─ OnHealthChanged.Broadcast() → Updates UI
        │   ├─ Checks if health <= 0
        │   └─ If dead:
        │       ├─ Sets bIsDead = true
        │       ├─ Calls HandleCharacterDeath()
        │       └─ OnCharacterDeath.Broadcast()
        │           ├─ Game mode receives death event
        │           └─ Checks win/lose conditions
        │
        ├─ Hit direction calculation (EDeltaHitDirection)
        │   ├─ Determines if forward/backward/left/right
        │   └─ Triggers appropriate reaction animation
        │
        └─ Knockback application (if applicable)
            └─ Character receives impulse force
```

---

## KEY STRUCT & DATA TYPES

### FSkillTypeWrapper
```cpp
USTRUCT(BlueprintType)
struct FSkillTypeWrapper
{
    GENERATED_BODY()
    
    // Array of 3 skills (UI slots)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EDeltaSkillType> SkillTypes;
    
    FSkillTypeWrapper()
    {
        SkillTypes.SetNum(3);  // Always 3 slots
    }
};

// Usage Example:
// SkillSetArray[0] = {Shield, Lightning, MeteorShower}  // Set 1
// SkillSetArray[1] = {Parrying, Slash, GroundCrack}     // Set 2
// SkillSetArray[2] = {Flashes, ArrowShot, RailGun}      // Set 3
```

### Input Mapping
```cpp
// InputDataAsset maps:
struct FInputActionConfig
{
    UInputAction* InputActionPtr;           // From Enhanced Input System
    EInputActionValueType ValueType;
    ETriggerEvent TriggerEvent;             // Triggered, Ongoing, Completed, etc.
    FName FunctionName;                     // "SkillFirst", "Move", "LockTarget", etc.
};

// Bound in SetupPlayerInputComponent():
EnhancedInput->BindAction(InputActionPtr, TriggerEvent, this, "SkillFirst");
// When action triggered → Calls ADeltaPlayableCharacter::SkillFirst()
```

---

## CRITICAL WEAK POINTS FOR ALLY SYSTEM

### 1. TEAM DISTINCTION (CombatComponent::GetIsOpponent)
**Current Implementation**: Hardcoded to check against playable targets
```cpp
// From CombatComponent, needs modification:
// Currently assumes: AttackerOwner is always player-controlled
// Assumption breaks with allies
```

### 2. AI TARGET SELECTION (ADeltaAIController::GetRandPlayableTarget)
```cpp
// Current: Only looks for ADeltaPlayableCharacter
TArray<ADeltaBaseCharacter*> TempArray;
for (ADeltaBaseCharacter* CachedActor : PlayableTargets)
{
    if (!CachedActor) continue;
    // Distances checked blindly - doesn't check if enemy or ally
    if (FVector::Distance(...) < MaxDistance)
    {
        TempArray.Add(CachedActor);
    }
}
```

### 3. POSSESSION SYSTEM
```cpp
// Current: Only one character possessed at a time
ADeltaPlayerController possesses ADeltaPlayableCharacter
// Problem: No mechanism for switching possession
// No multi-character control input
```

### 4. PLAYER CONTROLLER AWARENESS
```cpp
// Current: ADeltaPlayerController only knows ONE character
TWeakObjectPtr<ADeltaPlayableCharacter> OwningPlayerCharacter;

// Needed for allies:
// - Array of allies
// - Current active character tracking
// - Team state management
```

---

## REQUIRED MODIFICATIONS FOR ALLY SYSTEM

### 1. TEAM ENUMERATION
```cpp
// Add to DeltaEnumTypes.h:
UENUM(BlueprintType)
enum class ETeamAffiliation : uint8
{
    Player,      // Player-controlled allies
    Enemy,       // Enemy faction
    Neutral,     // NPCs, environment
};

// Add to ADeltaBaseCharacter:
UPROPERTY(EditAnywhere, Category = "Team")
ETeamAffiliation TeamAffiliation = ETeamAffiliation::Enemy;

// Modify CombatComponent::GetIsOpponent:
bool IsOpponent = (OwnerTeam != TargetTeam) && (TargetTeam != Neutral);
```

### 2. TEAM CONTROLLER
```cpp
// New class: ADeltaTeamController
class ADeltaTeamController : public APlayerController
{
    // Manages multiple allies
    TArray<ADeltaBaseCharacter*> TeamMembers;
    ADeltaBaseCharacter* CurrentControlledCharacter;
    
    void SwitchToCharacter(int32 Index);
    void SendCommandToAlly(int32 AllyIndex, ECommand Command);
    void UpdateTeamUI();
};
```

### 3. GAME MODE TEAM MANAGEMENT
```cpp
// Modify ADeltaBaseGameMode:
class ADeltaBaseGameMode : public AGameModeBase
{
    TArray<TWeakObjectPtr<ADeltaBaseCharacter>> PlayerTeam;
    TArray<TWeakObjectPtr<ADeltaBaseCharacter>> EnemyTeam;
    
    void UpdateTeamAffilations();
    bool IsPlayerWin(); // Check if all enemies dead AND allies alive
};
```

---

## ANIMATION SYSTEM DETAILS

### Montage Structure
Each skill is represented by an animation montage containing:

```
Montage: SK_Lightning_Montage
├─ Section 0: "Start" (0.0s - 0.5s)
│   └─ AnimNotify @ 0.3s: "PlaySkillAnimation_Start"
│
├─ Section 1: "Active" (0.5s - 1.2s)
│   └─ AnimNotifyState: "ActiveSkill"
│       ├─ BeginNotifyState @ 0.5s
│       │   └─ CombatComponent->BeginSkill()
│       │       └─ Creates USkillLightning instance
│       │
│       ├─ NotifyTick @ 0.6s, 0.7s, 0.8s ... (per frame)
│       │   └─ Check for overlaps on skill collision volumes
│       │       └─ CombatComponent->ApplySkillDamage()
│       │
│       └─ EndNotifyState @ 1.2s
│           └─ CombatComponent->EndSkill()
│
└─ Section 2: "Recovery" (1.2s - 1.5s)
    └─ AnimNotify @ 1.5s: "EndSkillAnimation"
        └─ Character->EndSkillAnimation()
            └─ CombatComponent->ReleaseSkill()
```

### Animation Events Sequence
1. **AnimNotify_ActiveSkill** → Single event, fires once
2. **AnimNotifyState_ActiveSkill** → Interval event, fires every tick during active window
3. Animation montage completes → Character returns to idle

---

## BLACKBOARD KEYS (AI)

Enemies use Behavior Tree Blackboard for state storage:

```cpp
// Common Blackboard Keys:
"TargetActor"           // ADeltaBaseCharacter* (current target)
"TargetLocation"        // FVector (where to move/aim)
"CurrentSkill"          // EDeltaSkillType (what to use next)
"AttackDelayTime"       // float (wait time)
"SpawnLocation"         // FVector (return point)
"SkillRange"            // float (how far skill reaches)
"IsPlayerClose"         // bool (target within range)
"IsAtSpawn"             // bool (at home position)
```

---

## COMPONENT INITIALIZATION ORDER

```
ADeltaBaseCharacter Constructor
    ├─ Calls CreateDefaultSubobject<>() for components
    └─ Components not fully initialized yet

ADeltaBaseCharacter::BeginPlay()
    ├─ All components now initialized
    ├─ HealthComponent initializes MaxHealth & CurrentHealth
    ├─ ManaComponent initializes mana pool
    ├─ CombatComponent caches owner reference
    ├─ Bind damage callback: TakeSkillDamage
    └─ Subclass (playable/enemy) specific setup

PossessedBy(AController)
    ├─ Controller set
    ├─ InputComponent setup (playable only)
    └─ AI setup (enemies - runs BT)
```

---

## KEY TECHNICAL PATTERNS

### 1. Weak Pointer Usage
```cpp
// Used extensively for non-owned references:
TWeakObjectPtr<ADeltaBaseCharacter> CurrentSkillTarget;

// Safe usage:
if (CurrentSkillTarget.IsValid())
{
    CurrentSkillTarget->TakeDamage(...);
}

// Why: Objects can be deleted without notification
// WeakPtr becomes nullptr automatically
```

### 2. Virtual Method Overriding
```cpp
// Base behavior in ADeltaBaseCharacter:
virtual void UpdateSkillTarget();       // Generic trace

// Playable override:
virtual void UpdateSkillTarget() override; // Camera-based trace

// Enemy override:
virtual void UpdateSkillTarget() override; // Use AI target
```

### 3. Broadcast Delegates
```cpp
// Define in header:
FOnCharacterDeath OnCharacterDeath;     // Multicast

// Broadcast in implementation:
OnCharacterDeath.Broadcast(this);

// Subscribe in game mode:
Character->OnCharacterDeath.AddDynamic(GameMode, &GameMode::HandleDeath);

// Result: Game mode automatically called when character dies
```

---

## DIFFICULTY SCALING IMPLEMENTATION

```cpp
// From DeltaEnemyCharacter::ApplyDifficultySettings()

UFrontGameUserSettings* UserSettings = UFrontGameUserSettings::Get();
EDeltaGameDifficulty Difficulty = UserSettings->GetCurrentGameDifficulty();

// Multiplier lookup table:
switch (Difficulty)
{
    case Easy:   HealthMult = 0.75f, DamageMult = 0.5f;  break;
    case Normal: HealthMult = 1.0f,  DamageMult = 1.0f;  break;
    case Hard:   HealthMult = 1.5f,  DamageMult = 1.5f;  break;
}

// Apply to components:
HealthComponent->SetMaxHealth(BaseHealth * HealthMult);
CombatComponent->SetDamageMultiplier(DamageMult);

// How it works:
// When damage dealt: Damage * CombatComponent->DamageMultiplier
// When damage taken: Damage * HealthComponent->DamageTakenMultiplier
```

