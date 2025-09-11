# Mission System Deep Dive

## Mission System Goals
1. Simplicity.
- Extendable to other systems. (Ex. Achievements, Tutorials)
- Mission Design should be as simple as possible & require low/no code.
2. Dynamic
- Mission Lists should be able to be changed at runtime.
3. Light Weight.
- Not to be a bloated mess with tentacles crawling through-out the code.

## Why this works
The MVVMC (Model View ViewModel Component) pattern is used to more easily share data between gameplay and UI without needing the UI to know anything about gameplay. 
Multiple UI components can all bind to the same View Model without neededing to know how or when the data is updated or evaluated. 
The View Model is the single source of truth for all the "Views" that bind to it. 
For instance, you can have multiple UI views that show data in a different form that all get updated when the View Model is updated.
Having a single subsystem that only broadcasts events with a Payload to the Mission Actor Component is Simple and easy to use.

## In Game Screenshot
![Mission System Screenshot](Images/MissionDisplayWithRewards.png)

## Code Map
| Class                                         | Role                                                           | Key Methods                                                                         | Jump                                                                                                                                                                       |
|-----------------------------------------------|----------------------------------------------------------------|-------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `UMissionDeliveryComponent` (Actor Component) | Owns mission state and handles mission filtering/processing.   | `InitializeActiveMission`, `SetActiveMission`, `CheckMissionContext`, `HandleEvent` | [H](Source/timbermvp/Public/Components/MissionDelivery/MissionDeliveryComponent.h)·[CPP](Source/timbermvp/Private/Components/MissionDelivery/MissionDeliveryComponent.cpp) |
| `UMissionViewModel` (View Model)              | Single Source of Truth for Mission Data. UI Facing.            |                                                                                     | [H](Source/timbermvp/Public/ViewModels/MissionViewModel.h)·[CPP](Source/timbermvp/Private/ViewModels/MissionViewModel.cpp)                                                                                                       |
| `UMissionEventSubsystem` (Game Subsystem)     | Broadcasts Payload to MDC Actor Component                      | `BroadcastMissionEvent`                                                             | [H](Source/timbermvp/Public/Subsystems/Events/CombatEventSubsystem.h)·[CPP](Source/timbermvp/Private/Subsystems/Events/CombatEventSubsystem.cpp)                                                                                                    |
| `UMissionList` (Data Asset)                   | Stores a set of Missions. Lives in UMissiongDeliveryComponent. |                                                                                     | [H](Source/timbermvp/Public/Data/DataAssets/MissionSystem/MissionList.h)                                                                                                   |
| `UMissionBase` (Data Asset)                   | Data + state for one mission                                   |                                                                                     | [H](Source/timbermvp/Public/Data/DataAssets/MissionSystem/MissionBase.h)                                                                                                   |
| `UMissionDisplayWidget` (View/UI)             | Displays Data / Handles Animations and Visuals                 |                                                                                     | [H](Source/timbermvp/Public/Data/DataAssets/MissionSystem/MissionBase.h)                                                                                                   |

## Features
1. Event Driven Mission System.
2. Easy Mission Design via in editor Data Assets.
2. Gameplay Tags are used for both mission design and event filtering.
3. View Model Data can be used outside of the Mission UI.
4. Coupling only to the View Model and Event Subsystems.

# Mission System Architecture
### Pattern Overview
![Pattern Overview](Images/Pattern.png)


### Mission Delivery System Initialization
![Initialization](Images/Initialization.png)


### Example Damage Event Pipeline
![Damage Event Pipeline](Images/DamageEventPipeline.png)

### Save/Load Mission State
![Save and Load](Images/SaveLoad.png)


## 🔗 Links
- [View Code on GitHub](https://github.com/robinnnnnn/timbermvp)
- [Return to Main Project Repo](https://github.com/yourname/yourrepo)
