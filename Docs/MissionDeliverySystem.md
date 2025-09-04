```mermaid
---
title: MVVMC Pattern
---
flowchart TD
    A(Data Mission Actor Component   -----)
    C[ModelView   ---]
    D[View  --]
    
    A --> |Send Data  --| C
    D --> |Binds Values  -- | C
    
```

```mermaid
---
title: Mission Delivery System - Initialization
---
sequenceDiagram
    participant GameMode
    participant Mission-ActorComponent
    participant ViewModel
    participant Mission-Display-UI
    
    GameMode->>GameMode: Initialize Missions (Load DA)
    GameMode->>Mission-ActorComponent: Assign Current Mission
    Mission-ActorComponent->>Mission-ActorComponent: Bind to CombatEventsSubsystem
    Mission-ActorComponent->>Mission-ActorComponent: Bind to BuildEventsSubsystem
    Mission-ActorComponent->>ViewModel: Structure Mission Data for Display
    Mission-Display-UI->>ViewModel: Binds to ViewModel
    ViewModel->>Mission-Display-UI: Updates Display

```

```mermaid

---
title: MDS - Combat Event Data Flow
---
    sequenceDiagram
    autonumber
    participant E as Enemy
    participant CES as CombatEventsSubsystem
    participant M as MissionActorComponent
    participant MVVM as MissionViewModel

    E->>E: TakeDamage()
    E->>CES: PublishDamageEvent()
    CES->>M: BroadcastEvent(Payload)
    M->>M: Filter (player? right EventTag? TagQuery match?)
    M->>MVVM: Increment mission progress, update ViewModel
```

## Required Classes
### MissionDataAsset - UDataAsset
  - Title - Destroy the Enemy
  - Description - Destroys X Carbonites.
  - Progress - float %
    - UI Progress Bar.
  - Rewards
    - 20 Parts - 10 Mechanism
  - Events Tag
    - Combat.Destroy
  - Context Tags
    - Enemy.Carbonite
  - Event Based Context
    - Destroy
      - int Count (How Many To Destroy)
      - FGameplayTag Weapon.Melle
        - What Type of Weapon to look for in the Context Tags.
  - Has a Specific GUID used for Player Saving/Progress.
        
### Mission List Data Asset
    - List of MissionDataAssets
    - Can be packaged for certain setups
        - Ex. Tutorials, Standard, End Game, Endless

### MissionViewModel
  - Structured Data for Display/Mission UI
  - UI Binds to this Class.
  - FText Title
  - FText Description
  - FText Progress
  - Float Progress
  - This is bound directly to from the Widgets that need it in BP's
    - Ex. MissionDisplayUI
### MissionDisplayUI
  - Displays Current Mission Goals and Progress
  - Display
    - Title
    - Brief Description
      - Context Dependent
      - Destroy 10 Carbonites with a Melee Weapon 
        - Count
        - Enemy Type
        - Weapon Type
    - Progress Bar
    - Rewards
### MissionActorComponent
  - Handles Logic for Mission Progress.
  - Updates Mission View Model.
  - Binds to CombatEventsSubsystem
    - CES Broadcasts at Every Damage Dealing Combat event with Payload.
    - Payload
      - Instigator
      - Target
      - Amount
      - ContextTags
        - Weapon
        - Damaged vs Destroyed.
  - Binds to BuildEventsSubsystem
    - BES Broadcasts at Every Build Event
  - Gets Called at Start of Game
  - Assigns Current Mission

### CombatEventsSubsystem & BuildEventsSubsystem
  - Gets Called after Every Combat Event or Build Event with Payload
  - PublishDamageEvent
  - PublishKillEvent
  - Uses Payload to Broadcast Relevant Data back to the MissionActorComponent

### SaveLoadSystem
  - Saves Overall Mission Status
  - Tracks Missions Completed 


### Objective Display
- Destroy [10] Carbonites
- Destroy [10] Carbonites with [Hammer]
- Destroy [10] Carbonites with [Rifle]
- Destroy [10] Carbonites with [Traps]
- Destroy [10] [Grunts] with [Traps]
- Destroy [10] [Brutes] with [Rifle]
- Build [3] [Structures]
- Build [3] [Components]
- Build [3] [Traps]
- Defeat Wave with only [Trap]
- Ensure the Data Seed Takes 0 Damage for [1] Wave

- Objectives should be able to Stack. IE, multiple objects per mission.

