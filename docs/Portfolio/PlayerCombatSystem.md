# Player Combat System / Abilities (Deep Dive)

The Player combat system is made of an Actor Component that handles the players abilties dependent on which weapon is equipped.
Each weapon has encapsulated abilities that control their own behavior. Abilties can be swapped out at runtime and can be created by 
subclassing the UWeaponAbility class.

---

## 🔑 Key Features
- All Ability logic is encapsulated.
- Abilities are stored on the Weapon.
- Combat Component known nothing about ability logic.

---

## 📂 Code Map
| Class                                  | Role                                    | Key Methods         | GitHub      |
|----------------------------------------|-----------------------------------------|---------------------|-------------|
| `UCombatComponent` (Actor Component)   | Manages ability Cooldowns and Resources | `ImportantMethod()` | [Code](...) |
| `UWeaponAbilityBase` (Ability UObject) | Defines the abilities Logic.            | `Execute()`         | [Code](...) |

---

## 🗂 System Architecture

- Insert **Mermaid diagram** (flowchart or sequence diagram).
- One diagram is enough here — keep others for an *Architecture Overview* page.
- Add 2–3 sentences explaining the flow.