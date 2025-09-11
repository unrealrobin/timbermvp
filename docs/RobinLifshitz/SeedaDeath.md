
# Seeda Death
Description: This sequence describes how the game reacts when Seeda's health reaches zero, triggering player death logic, AI freeze, and the Death UI screen transition.
## Seeda Death Process

```mermaid
sequenceDiagram
    participant Seeda
    participant Kip
    participant Kip_Controller
    participant GameMode
    participant HUD
    
    note over Seeda: Seedas Health Drops below 0
    Seeda->>HUD: OnSeedaDeathUI.Broadcast(bIsSeedaDestroyed = true)
    HUD->>HUD: UpdateDeathUIReason_SeedaDestroyed();
    Seeda->>Kip: OnSeedaDeath.Broadcast(bIsSeedaDestroyed = true)
    Seeda->>Seeda: Destroy();
    note over Kip: Kip Hears OnSeedaDeath Broadcast
    Kip->>Kip: HandlePlayerDeath(bIsSeedaDestroyed = true)
    Kip->>Kip_Controller: HandlePlayerDeath_DelegateHandle.Broadcast(bIsPlayerDeadNow = true)
    Kip->>GameMode: HandlePlayerDeath_DelegateHandle.Broadcast(bIsPlayerDeadNow = true)
    par DualBroadcast 
    note over GameMode: GameMode Hears HandlePlayerDeath_DelegateHandle Broadcast
    GameMode->>GameMode: FreezeAi()
    note over Kip_Controller: Kip_Controller Hears HandlePlayerDeath_DelegateHandle Broadcast
    Kip_Controller->>Kip_Controller: Internal Logic()
    end
    Kip_Controller->>HUD: HandleDeathUi()
    note over Kip_Controller: Can Only Update Death UI Reason if Default
    HUD->>HUD: ShowDeathUi()
    note right of HUD: Death UI Resets Reasons on Button Press.

```

---
Note:
1. When the Data Seed (Seeda) dies, the play is also destroyed directly after. The player also tries to update the 
   death reason ui so we have a check to make sure it can only be set when its the default reason.