# Player Combat Ability System

The Player combat system is comprised of an Actor Component that handles the players abilties dependent on which weapon is equipped.
Each weapon has encapsulated abilities that control their own behavior. Abilties can be swapped out at runtime and can be created by 
subclassing the UWeaponAbility class. Abilities control the Life Time of the Instantiated Ability Object.

## Goals
1. Encapsulate Ability Logic
- All ability logic lives in the ability.
- All VFX/Sounds/Animations are Encapsulated in the ability class.
2. Drag and Drop Actor Component
- Any Playable Character can have the Component added to them.
3. Dynamic Ability
- Abilities should be able to be swapped at Runtime, upgraded, etc.
---

## Code Map
<!-- blank line above! -->

| Class                                 | Role                                                              | Key Methods                                                                                                 | GitHub                                                                                                                                                                                                                                                       |
|---------------------------------------|-------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `UCombatComponent` (Actor Component)  | Manages ability Cooldowns and Resources                           | `HandlePrimaryAbility()`, `HandleSecondaryAbility()`, `ValidateWeaponAbility()`, `GenerateAbilityContext()` | [H](https://github.com/unrealrobin/timbermvp/blob/main/Source/timbermvp/Public/Components/Combat/CombatComponent.h) - [CPP](https://github.com/unrealrobin/timbermvp/blob/main/Source/timbermvp/Private/Components/Combat/CombatComponent.cpp)               |
| `UWeaponAbilityBase` (Ability UObject) | Defines the abilities Logic.                                      | `Execute()`, `HandleCleanup()`                                                                              | [H](https://github.com/unrealrobin/timbermvp/blob/main/Source/timbermvp/Public/Weapons/Abilities/WeaponAbilityBase.h) - [CPP](https://github.com/unrealrobin/timbermvp/blob/main/Source/timbermvp/Private/Weapons/Abilities/WeaponAbilityBase.cpp)           |
| `UBasicSlash` (Melee Primary Ability) | The primary ability for a melee weapon. This ability has a Combo. | `Execute()`, `ResetComboData()`                                                                             | [H](https://github.com/unrealrobin/timbermvp/blob/main/Source/timbermvp/Public/Weapons/Abilities/MeleeWeapon/BasicSlash.h) - [CPP](https://github.com/unrealrobin/timbermvp/blob/main/Source/timbermvp/Private/Weapons/Abilities/MeleeWeapon/BasicSlash.cpp) |

---

## Key Features
- All Ability logic is encapsulated.
- Abilities are stored on the Weapon.
- Combat Component knows nothing about ability logic. Simply calls its `Execute()` method.
- Abilities use a context payload to communicate with the Combat Component.

---
## System Architecture


## Pattern
![Pattern Image](./Images/CombatComponentPattern.png)

The Combat Component is the main entry point for all abilities. This is helpful by reducing the amount of process logic and evaluation that would have
otherwise been written in the ability, character class, or controller. Now, the controller sends all combat/weapon related events to the Combat Component.

## Primary Ability Sequence
![Ability Sequence](./Images/PrimaryAbilitySequence.png)

The key idea here, is to have a base UObject Class that encapsulated the all the data for the ability. When an ability is requested, it is first validated, allowing
us to not have to instantiate the ability if it is not valid (valid means the ability is available and ready to use). If the ability is valid, then it is
instantiated and the ability is executed. The ability cleans itself up after it is done executing. It removes all references to itsels from the combat component
and then is garbage collected.

