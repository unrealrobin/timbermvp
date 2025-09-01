```mermaid

flowchart TD
    A(MissionComponent) -->|Actor Component| B(Player Character)
    C[Data Asset] --> D[Holds Mission Data.]
    E(View Model Class) --> |Holds Data to be Used by UI| F(W/WBPMission Display)
```

```mermaid

sequenceDiagram
    participant WaveSubsystem
    participant Mission-DataAsset
    participant ViewModel
    participant Mission-Display-UI
    participant Player
    participant Mission-ActorComponent
    participant SaveLoadSystem
    
    WaveSubsystem->>Mission-DataAsset: Get Mission Data
    Mission-DataAsset->>ViewModel: Initialize Required Data for Display
    ViewModel->>Mission-Display-UI: Set Display Data
    
    Player->>Mission-ActorComponent: Kills Enemys / Event Fires.
    Mission-ActorComponent->>Mission-ActorComponent: Updates Mission Progress
    Mission-ActorComponent->>ViewModel: Updates View Model
    ViewModel->>Mission-Display-UI: Updates Display
    Mission-Display-UI->>SaveLoadSystem: Save Mission Data
    SaveLoadSystem->>Mission-DataAsset: Save Mission Data
    

```


```mermaid
classDiagram
    MissionDataAsset 
    
```