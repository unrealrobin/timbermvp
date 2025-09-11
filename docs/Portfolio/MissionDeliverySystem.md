# Mission System Code Map

## Mission System Goals
1. Simplicity.
   2. Extendable to other systems. (Ex. Achievements, Tutorials)
   3. Mission Design should be as simple as possible & require no code.
3. Dynamic.
   4. Mission Lists should be able to be changed at runtime.
5. Light Weight.

## Why this works
The MVVMC (Model View ViewModel Component) pattern is used to more easily share data between gameplay and UI without needing the UI to know anything about gameplay. Multiple UI components can all bind to the same View Model without neededing to know how or when the data is updated or evaluated. The View Model
is the single source of truth for all the "Views" that bind to it. This is a highly flexible and scalable design. For instance, you can have multiple UI views that show data in a different form that all get updated when the View Model is updated.

## In Game Screenshot
![Mission System Screenshot](./Images/MissionDisplayWithRewards.png)

## Code Map
| Class                                         | Role                                                           | Key Methods                                                                         | Jump                                                                                                                                                                       |
|-----------------------------------------------|----------------------------------------------------------------|-------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `UMissionDeliveryComponent` (Actor Component) | Owns mission state and handles mission filtering/processing.   | `InitializeActiveMission`, `SetActiveMission`, `CheckMissionContext`, `HandleEvent` | [H](Source/timbermvp/Public/Components/MissionDelivery/MissionDeliveryComponent.h)·[CPP](Source/timbermvp/Private/Components/MissionDelivery/MissionDeliveryComponent.cpp) |
| `UMissionViewModel` (View Model)              | Single Source of Truth for Mission Data. UI Facing.            |                                                                                     | [H](Source/timbermvp/Public/ViewModels/MissionViewModel.h)·[CPP](Source/timbermvp/Private/ViewModels/MissionViewModel.cpp)                                                                                                       |
| `UCombatEventSubsystem` (Game Subsystem)      | Broadcasts Payload to MDC Actor Component                      | `BroadcastCombatEvent`                                                              | [H](Source/timbermvp/Public/Subsystems/Events/CombatEventSubsystem.h)·[CPP](Source/timbermvp/Private/Subsystems/Events/CombatEventSubsystem.cpp)                                                                                                    |
| `UBuildEventSubsystem` (Game Subsystem)       | Broadcasts Payload to MDC Actor Component                      | `BroadcastBuildEvent`                                                               | [H](Source/timbermvp/Public/Subsystems/Events/BuildEventSubsystem.h)·[CPP](Source/timbermvp/Private/Subsystems/Events/BuildEventSubsystem.cpp)                                                                                                      |
| `UMissionList` (Data Asset)                   | Stores a set of Missions. Lives in UMissiongDeliveryComponent. |                                                                                     | [H](Source/timbermvp/Public/Data/DataAssets/MissionSystem/MissionList.h)                                                                                                   |
| `UMissionBase` (Data Asset)                   | Data + state for one mission                                   |                                                                                     | [H](Source/timbermvp/Public/Data/DataAssets/MissionSystem/MissionBase.h)                                                                                                   |

## Features
1. Event Driven Mission Ssytem.
2. Easy Mission Design via in editor Data Assets.
2. Gameplay Tags are used for both mission design and event filtering.
3. View Model Data can be used outside of the Mission UI.
4. Coupling only to the View Model and Event Subsystems.

# Mission System Architecture
### Pattern Overview
```mermaid
flowchart LR
    A(Data Mission Actor Component   -----)
    C[ModelView -]
    D[UI ele. 1 -]
    G[UI ele. 2 -]
    H[UI ele. 3 -]
    
    E{{Event Subsystem A --}}
    F{{Event Subsystem B --}}
    
    E -.-> A
    F -.-> A
    A --> |Process & Update Values  ---| C
    C <--> |Bound Values  -- | D
    C <--> |Bound Values  -- | G
    C <--> |Bound Values  -- | H
    
```

### Mission Delivery System Initialization
```mermaid
sequenceDiagram
    participant GameMode
    participant Mission-ActorComponent
    participant ViewModel
    participant Mission-Display-UI
    
    GameMode->>GameMode: Initialize Missions (Load DA)
    GameMode->>Mission-ActorComponent: Assign Current Mission
    Mission-ActorComponent->>Mission-ActorComponent: Bind to Event Subsystems
    Mission-ActorComponent->>ViewModel: Structure Mission Data for Display
    ViewModel->>Mission-Display-UI: Updates Display

```

### Example Damage Event Pipeline
```mermaid
sequenceDiagram
    participant E as Event Dispatcher
    participant A as Event Subsystem A
    participant B as Mission-ActorComponent
    participant C as ViewModel
    participant D as Mission-Display-UI
    
    E->> E: Generate Payload.
    E->> A: Fires event with Payload.
    A->> A: Broadcasts event to listeners.
    B->> B: Filter Payload via GameplayTags
    B->> B: Process Payload & Mission State
    B->> C: Updates ViewModel
    C->> C: Structures Data for Display
    C->> D: Updates Display Values
    D->> D: Updates Display

```
### Save/Load Mission State
```mermaid

---
title: MDS - Save/Load Mission Completion State
---
sequenceDiagram
    
participant A as GameMode
participant B as SaveLoadSystem
participant C as Mission-ActorComponent

A ->> B: Save Game
B ->> B: Save Player State
B ->> C: Get Completed Mission GUID's (TArray)
C ->> B: Save Completed Mission GUID's (TArray)

A ->> B: Load Game
B ->> B: Load Player State
B ->> C: Populate Mission GUID's (MDS Actor Component)
C ->> C: Assign Uncompleted Mission from Missions List

    
```


## 🔗 Links
- [View Code on GitHub](https://github.com/robinnnnnn/timbermvp)
- [Return to Main Project Repo](https://github.com/yourname/yourrepo)
