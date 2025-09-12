```mermaid
flowchart LR
    A(Player Combat Component)
    B(PlayerController)
    C(Weapon Ability Object)
    D(Player Character)
    E(Weapon)

    
    
    D --> |Player Input| B
    B --> |Combat Event|A
    A --> |Process Ability|E
    A --> |Execute Ability| C

```

```mermaid
sequenceDiagram 
    participant A as PlayerController
    participant B as CombatComponent
    participant C as Weapon
    participant D as WeaponAbility
    
    A->>A: Primary Ability Input Event
    A->>B: HandlePrimaryAbility()
    B-->C: Instantiate Ability
    B-->B: ValidateWeaponAbility()
    B-->B: GenerateAbilityContext()
    B->>D: Execute()
    D->>D: HandleCleanup()
    
    
```