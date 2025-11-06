# Delta Project - System Architecture Diagrams
## Visual Reference for Ally System Implementation

---

## 1. CHARACTER POSSESSION & CONTROL FLOW

### Current Single-Character System
```
┌─────────────────────────────────────────────────────────────┐
│                    INPUT DEVICES                            │
│          (Keyboard, Mouse, Gamepad)                         │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│            ADeltaPlayerController                           │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ Enhanced Input System                                  │ │
│  │ ├─ InputDataAsset (action mappings)                    │ │
│  │ ├─ Input binding to character functions               │ │
│  │ └─ Input enable/disable control                       │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ State Management                                       │ │
│  │ ├─ Current focused character (weak ref)                │ │
│  │ ├─ UI connection                                       │ │
│  │ └─ Target detection callbacks                         │ │
│  └────────────────────────────────────────────────────────┘ │
└────────────────────┬────────────────────────────────────────┘
                     │ Possesses
                     ▼
          ┌──────────────────────┐
          │ ADeltaPlayableChar   │
          │ ("Back")             │
          │                      │
          │ ┌──────────────────┐ │
          │ │ Camera System    │ │
          │ │ SpringArm        │ │
          │ │ Camera Component │ │
          │ └──────────────────┘ │
          │                      │
          │ ┌──────────────────┐ │
          │ │ Input Handling   │ │
          │ │ Move, Look       │ │
          │ │ Skills 1,2,3     │ │
          │ │ LockTarget       │ │
          │ └──────────────────┘ │
          │                      │
          │ ┌──────────────────┐ │
          │ │ Combat System    │ │
          │ │ Skill execution  │ │
          │ │ Animation        │ │
          │ │ Targeting        │ │
          │ └──────────────────┘ │
          └──────────────────────┘
```

### Proposed Multi-Character System (For Allies)
```
┌─────────────────────────────────────────────────────────────┐
│                    INPUT DEVICES                            │
│          (Keyboard, Mouse, Gamepad)                         │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│            ADeltaPlayerController (EXTENDED)                │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ Team Management                                        │ │
│  │ ├─ TArray<ADeltaBaseCharacter*> TeamMembers           │ │
│  │ ├─ CurrentControlledIndex                             │ │
│  │ ├─ SwitchToCharacter(Index)                           │ │
│  │ └─ SendCommandToAlly(Index, Command)                  │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ Input System (ENHANCED)                                │ │
│  │ ├─ InputDataAsset (action mappings)                    │ │
│  │ ├─ Character-specific keybinds                        │ │
│  │ ├─ Team switching binds (E, R, T, etc)               │ │
│  │ └─ Quick command binds                                │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ Team UI & Communication                                │ │
│  │ ├─ Team status display                                │ │
│  │ ├─ Health bars for all members                        │ │
│  │ ├─ Target info distribution                          │ │
│  │ └─ Formation/tactic display                          │ │
│  └────────────────────────────────────────────────────────┘ │
└────────────────────┬────────────────────────────────────────┘
                     │
          ┌──────────┴──────────┐
          │                     │
          ▼ Possesses           ▼ (Others idle/AI)
    ┌──────────────┐        ┌──────────────┐
    │ PlayerChar   │        │ AllyChar 1   │
    │ Active Input │        │ AI Controlled│
    └──────────────┘        └──────────────┘
                                   │ ...
                            ┌──────────────┐
                            │ AllyChar N   │
                            │ AI Controlled│
                            └──────────────┘
```

---

## 2. COMBAT SYSTEM INTERACTION CHAIN

```
SKILL EXECUTION PATH:

Character A (Attacker)
    │
    ├─ CombatComponent
    │   ├─ BeginSkill(SkillClass)
    │   │   └─ Creates: USkillBase* instance
    │   │       └─ InitializeCollisions()
    │   │           └─ Creates collision volumes
    │   │
    │   └─ ApplySkillDamage(Target, Damage)
    │       └─ HealthComponent->TakeDamage()
    │
    └─ HealthComponent (Defender)
        ├─ CurrentHealth -= Damage
        ├─ OnHealthChanged.Broadcast()
        │   └─ Updates UI
        │
        └─ If health <= 0:
            ├─ HandleCharacterDeath()
            ├─ OnCharacterDeath.Broadcast()
            │   └─ Game Mode processes death
            │
            ├─ Disable input
            ├─ Play death animation
            └─ Set bIsDead = true


TARGETING CHAIN:

Skill User
    │
    ├─ CombatComponent
    │   ├─ GetSkillTargetActor()
    │   │   ├─ Uses CurrentSkillTarget (character ref)
    │   │   └─ Validates: Alive, In range, Is opponent
    │   │
    │   └─ GetSkillTargetLocation()
    │       ├─ Returns explicit location OR
    │       └─ Returns target's current position
    │
    └─ During Skill:
        └─ Collision volumes check for overlap
            └─ Auto-targets overlapping characters

AI TARGETING (Enemy Specific):

ADeltaAIController
    │
    ├─ Runs Behavior Tree
    │   │
    │   ├─ BTTask_TargetUpdate
    │   │   └─ GetRandPlayableTarget()
    │   │       ├─ Scans world for ADeltaPlayableCharacter
    │   │       ├─ Filters by MaxDistance
    │   │       └─ Sets Blackboard["TargetActor"]
    │   │
    │   └─ BTTask_AttackTarget
    │       └─ Enemy->PlaySkillAnimation(CurrentSkill)
    │           ├─ Updates skill target
    │           └─ Plays montage
    │
    └─ (PROBLEM: Only targets "PlayableCharacter" type)
        (SOLUTION: Need team affiliation check)
```

---

## 3. SKILL SYSTEM ARCHITECTURE

```
SKILL INHERITANCE HIERARCHY:

USkillBase (Abstract)
    │
    ├─ USkillProjectile
    │   ├─ Creates AProjectileBase actor
    │   ├─ Fires projectile from attacker
    │   ├─ Projectile checks overlap
    │   └─ Applies damage on hit
    │
    ├─ USkillCollisionAttack
    │   ├─ Creates collision volumes
    │   ├─ Checks overlap during active window
    │   └─ Multi-hit support
    │
    ├─ USkillParrying
    │   ├─ Create defensive collision
    │   ├─ Reduces incoming damage
    │   └─ Can reflect projectiles
    │
    ├─ USkillShield
    │   ├─ Creates damage absorption layer
    │   ├─ Depletes before health
    │   └─ Can protect allies (if implemented)
    │
    └─ [19+ other skill types]


SKILL LIFECYCLE (Per Animation Frame):

Montage Plays
    │
    ├─ 0.0s - Start Section
    │   └─ AnimNotify: PlaySkillAnimation_Start
    │       └─ CombatComponent->BeginSkill(SkillClass)
    │           └─ USkillBase->BeginSkill()
    │               └─ Initialize collision volumes
    │
    ├─ 0.5s - Active Section Begins
    │   ├─ AnimNotifyState_ActiveSkill -> Begin
    │   │   └─ Skill ready to damage
    │   │
    │   └─ AnimNotifyState_ActiveSkill -> Tick (every frame)
    │       └─ Check overlaps on collision volumes
    │           └─ For each overlap:
    │               └─ CombatComponent->ApplySkillDamage()
    │                   ├─ Damage = BaseDamage * DamageMultiplier
    │                   └─ Apply knockback
    │
    ├─ 1.2s - Active Section Ends
    │   └─ AnimNotifyState_ActiveSkill -> End
    │       └─ CombatComponent->EndSkill()
    │           └─ Stop damage checks
    │
    └─ 1.5s - Animation Complete
        └─ AnimNotify: EndSkillAnimation
            └─ CombatComponent->ReleaseSkill()
                ├─ Delete skill object
                └─ Reset character state


SKILL DATA STORAGE:

USkillDataAsset
    ├─ EDeltaSkillType Type
    │   └─ Identifies skill (Lightning, Shield, etc)
    │
    ├─ TSubclassOf<USkillBase> SkillClass
    │   └─ Blueprint class to instantiate
    │
    ├─ UAnimMontage* SkillMontage
    │   └─ Animation to play
    │
    ├─ FSkillStats
    │   ├─ float Damage
    │   ├─ float Range
    │   ├─ float Cost (mana)
    │   ├─ float Cooldown
    │   └─ float Duration
    │
    └─ Effect Parameters
        ├─ VFX references
        ├─ SFX references
        └─ Gameplay parameters
```

---

## 4. AI CONTROLLER & BEHAVIOR TREE FLOW

```
ADeltaEnemyCharacter Spawned
    │
    ▼
Possess(ADeltaAIController)
    │
    ├─ BehaviorTree loaded
    └─ BlackboardAsset initialized
        │
        ▼
BEHAVIOR TREE EXECUTION (Continuous Loop):

Root Selector (picks first successful child)
│
├─ Decorator: IsAlive?
│   │
│   └─ Sequence (must all succeed)
│       │
│       ├─ Task: UpdateSpawnLocation
│       │   └─ Records initial position
│       │
│       ├─ Task: TargetUpdate
│       │   ├─ GetRandPlayableTarget(1000 units)
│       │   │   ├─ Scans for ADeltaPlayableCharacter
│       │   │   ├─ Filters by distance
│       │   │   └─ Picks random if multiple
│       │   │
│       │   └─ Sets Blackboard["TargetActor"]
│       │
│       ├─ Decorator: IsPlayableClose?
│       │   │   (Checks distance to target)
│       │   │
│       │   └─ Sequence
│       │       │
│       │       ├─ Task: MoveToDynamicRadius
│       │       │   ├─ Calculates skill range
│       │       │   └─ Moves to optimal distance
│       │       │
│       │       ├─ Task: SetSkill
│       │       │   └─ Enemy->SetCurrentSkill()
│       │       │       └─ Random from SkillDataAssets
│       │       │
│       │       ├─ Task: WaitAttackDelay
│       │       │   └─ Waits before attacking
│       │       │
│       │       └─ Task: AttackTarget
│       │           └─ Enemy->PlaySkillAnimation()
│       │               └─ Executes attack
│       │
│       └─ Decorator: IsAtSpawnLocation?
│           │   (If target lost or too far)
│           │
│           └─ Task: MoveToLocation
│               └─ Return to spawn point
│
└─ Task: Die (if not alive)
    └─ Stop all behavior


BLACKBOARD STRUCTURE:

Key Name              Type              Updated By
────────────────────────────────────────────────────────────
"TargetActor"         Object*           BTTask_TargetUpdate
"TargetLocation"      Vector            BTTask_MoveToLocation
"CurrentSkill"        Enum              BTTask_SetSkill
"SkillRange"          Float             Calculated
"AttackDelayTime"     Float             BTTask_WaitAttackDelay
"SpawnLocation"       Vector            BTTask_UpdateSpawnLocation
"IsPlayerClose"       Bool              BTDecorator check
"IsAtSpawn"           Bool              BTDecorator check
"IsFlying"            Bool              Character state
```

---

## 5. GAME MODE STATE MANAGEMENT

```
GAME INITIALIZATION:

1. Level Loads
   └─ ADeltaBaseGameMode created

2. BeginPlay()
   ├─ Get all actors in world
   ├─ Classify into:
   │   ├─ TArray<PlayableCharacters>  ← All ADeltaPlayableCharacter
   │   └─ TArray<EnemyCharacters>     ← All ADeltaEnemyCharacter
   │
   ├─ Subscribe to death events
   │   ├─ For each PlayableCharacter:
   │   │   └─ OnCharacterDeath += HandlePlayableCharacterDeath
   │   │
   │   └─ For each EnemyCharacter:
   │       └─ OnCharacterDeath += HandleEnemyCharacterDeath
   │
   └─ Wait for GameStart() call


GAME STATES:

┌──────────────┐
│ WaitForStart │  (Initial state)
└──────┬───────┘
       │ GameStart() called
       ▼
┌──────────────┐
│   Playing    │  (Battles occur)
└──────┬───────┘
       │ All enemies dead OR player dead
       ▼
┌──────────────┐
│    Result    │  (Victory/Defeat shown)
└──────────────┘


DEATH EVENT HANDLING:

Enemy Dies:
    OnCharacterDeath.Broadcast()
        │
        ├─ Game Mode receives event
        ├─ Removes from EnemyCharacters array
        │
        └─ Check: Are all enemies dead?
            └─ Yes: GameEnd() with victory
                └─ Show results screen

Player Dies:
    OnCharacterDeath.Broadcast()
        │
        ├─ Game Mode receives event
        ├─ Removes from PlayableCharacters array
        │
        └─ Check: Are all players dead?
            └─ Yes: GameEnd() with defeat
                └─ Show results screen

(PROBLEM: Only checks ADeltaPlayableCharacter death)
(SOLUTION: Need to include allies in victory check)
```

---

## 6. INTERACTION DIAGRAM - WHO TALKS TO WHOM

```
CONTROLLER ◄──────────────┐
    │                     │
    │ Possesses           │ (Callbacks)
    │                     │
    ▼                     │
CHARACTER ◄──────────────┘
    │ Uses components
    ├─────────────────────┬─────────────────┬──────────────┐
    │                     │                 │              │
    ▼                     ▼                 ▼              ▼
CombatComponent    HealthComponent    ManaComponent  MotionWarping
    │                   │                 │
    │                   │ On Death:       │
    │                   └──────────┬──────┘
    │                              │
    │                              ▼
    │                        Game Mode
    │                              │
    └──────────────────────────────┘
       Creates/destroys skills

SKILL OBJECT:
    │
    └─ CombatComponent (owner)
        └─ Accesses target info from Character
            ├─ GetCurrentSkillTarget()
            └─ GetSkillTargetLocation()


INPUT FLOW:
    │
    └─ Player Input
        │
        ├─ PlayerController
        │   └─ Possess: ADeltaPlayableCharacter
        │       └─ SetupPlayerInputComponent()
        │           └─ Bind: SkillFirst → SkillFirst()
        │               └─ Direct to Character function
        │
        └─ Character receives input
            └─ Directly handles (no delegation)


AI FLOW:
    │
    └─ Behavior Tree
        │
        ├─ Runs in: ADeltaAIController
        │
        └─ Calls: ADeltaEnemyCharacter methods
            ├─ SetCurrentSkill()
            ├─ PlaySkillAnimation()
            └─ UpdateSkillTarget()
```

---

## 7. PROPOSED ALLY SYSTEM ARCHITECTURE

```
NEW CLASS: ADeltaAllyCharacter
    ├─ Inherits: ADeltaBaseCharacter
    ├─ Has: AllyRole (Support, Damage, Tank)
    └─ Controlled by: ADeltaAllyController


NEW CLASS: ADeltaAllyController
    ├─ Inherits: AAIController
    ├─ Runs: AllyBehaviorTree
    ├─ Commands: List<EAllyCommand>
    │   ├─ Attack (specified enemy)
    │   ├─ Defend (position)
    │   ├─ Support (heal ally)
    │   └─ Retreat (to spawn)
    │
    └─ Perception: Sees player targets


TEAM AFFILIATION SYSTEM:
    │
    ├─ ADeltaBaseCharacter.TeamAffiliation
    │   ├─ Player (playable allies)
    │   ├─ Enemy (hostile)
    │   └─ Neutral (NPCs)
    │
    ├─ Modified: CombatComponent::GetIsOpponent()
    │   └─ Now checks: OwnerTeam != TargetTeam
    │
    └─ AI TargetUpdate modified:
        └─ Looks for team != own team


ENHANCED PLAYER CONTROLLER:
    │
    ├─ TArray<ADeltaBaseCharacter*> TeamMembers
    ├─ int CurrentControlledIndex
    │
    ├─ SwitchToCharacter(Index)
    │   ├─ Unpossess current
    │   ├─ Possess new character
    │   └─ Update UI/camera
    │
    └─ SendCommandToAlly(Index, Command)
        └─ Ally controller receives command


FORMATION SYSTEM (Optional):
    │
    ├─ Leader (player-controlled)
    ├─ Followers (allies)
    │   └─ Maintain: Position relative to leader
    │
    └─ Tactics
        ├─ Aggressive (attack with leader)
        ├─ Defensive (protect leader)
        └─ Ranged (stay back)
```

---

## 8. DATA FLOW DIAGRAMS

### Skill Execution Data Flow
```
Input
  │ KeyPressed
  ▼
Character::SkillFirst()
  │ Calls
  ▼
StartWaitingSkill(KeyIndex)
  ├─ Retrieves: SkillSetArray[CurrentSetIndex][KeyIndex]
  │ Returns: EDeltaSkillType
  │
  └─ FindSkillDataAsset(SkillType)
      ├─ Searches: SkillDataAssets array
      └─ Returns: USkillDataAsset*
          │ Contains: Montage, Damage, Cost, SkillClass
          │
          └─ Validates: Mana available
              └─ Broadcasts: OnSelectSkill (UI updates)


ExecuteSkill()
  │
  ├─ PlaySkillAnimation(SkillType)
  │   └─ Gets montage from SkillDataAsset
  │       └─ GetMesh()->GetAnimInstance()->Montage_Play()
  │
  └─ Sets CurrentStatus = Skill
      │
      └─ Montage Plays:
          │
          ├─ AnimNotify_ActiveSkill fires
          │   └─ CombatComponent->BeginSkill(SkillClass)
          │       ├─ NewObject<USkillBase>(this, SkillClass)
          │       └─ Skill->BeginSkill()
          │           └─ Initialize (collision, VFX, etc)
          │
          └─ AnimNotifyState_ActiveSkill (each frame)
              ├─ Performs overlap checks
              ├─ Collects hit actors: HitActors[]
              │
              └─ For each HitActor:
                  └─ CombatComponent->ApplySkillDamage()
                      ├─ Gets: SkillDataAsset->Damage
                      ├─ Multiplies: Damage * DamageMultiplier
                      │
                      └─ UGameplayStatics::ApplyDamage()
                          └─ Target::TakeSkillDamage() callback
                              └─ HealthComponent->TakeDamage()
```

### Damage Reception Data Flow
```
Damage Event
  │
  ▼
ADeltaBaseCharacter::TakeSkillDamage()
  │
  ├─ Calls: HealthComponent->TakeDamage(Damage, Causer)
  │
  ├─ HealthComponent processing:
  │   ├─ CurrentHealth -= Damage * DamageTakenMultiplier
  │   ├─ Clamps: CurrentHealth = Max(0, CurrentHealth)
  │   │
  │   ├─ Broadcasts: OnHealthChanged
  │   │   └─ UI updates health bar
  │   │
  │   └─ If health <= 0:
  │       └─ Sets: bIsDead = true
  │
  ├─ Back in Character callback:
  │   ├─ Calculates: EDeltaHitDirection
  │   │   └─ From damage location vs character forward
  │   │
  │   ├─ Applies knockback (if applicable)
  │   │   └─ Character physics impulse
  │   │
  │   └─ Plays reaction animation
  │       └─ Based on hit direction
  │
  └─ If dead:
      ├─ Calls: HandleCharacterDeath()
      │   ├─ Disables input
      │   ├─ Plays death montage
      │   └─ Disables collision
      │
      └─ Broadcasts: OnCharacterDeath
          └─ Game Mode::HandleCharacterDeath()
              └─ Updates win/lose conditions
```

