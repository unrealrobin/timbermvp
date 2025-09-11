```mermaid
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
```mermaid
sequenceDiagram
    participant E as Caller
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