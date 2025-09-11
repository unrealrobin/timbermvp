# Build System (Deep Dive)

The Build System is a core feature of the game. The player can build structures, components and traps that all snap together. The Build System Component is responsible
for all logic related to how the player can build the structures and how each structure interacts with the rest of the world.

---

## 🔑 Key Features
- Walls and Floors Snap Together via a Quadrant & Snap Point System.
- Traps & Component Snap to Walls and Floors, Utilizing open "Slots".
- Save and Load using GUID's to spawn the same buildable at a later time.
- Payment, Refunding, Deletion of buildables.
- Incorrect Placement visual representation.

---

## 📂 Code Map
| Class                                            | Role                                                                                                             | Key Methods | GitHub                                                                      |
|--------------------------------------------------|------------------------------------------------------------------------------------------------------------------|-------------|-----------------------------------------------------------------------------|
| `UBuildSystemManagerComponent` (Actor Component) | Handles Spawning, Placement and Snapping of all Buildables. (Structures, Components, Traps)                      | `ImportantMethod()` | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |
| `ABuildableBase` (Actor)                         | Base class for any buildable class.                                                                              | `ImportantMethod()` | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |
| `AComponentBase` (Actor)                         | Base Class for a specialized type of Buildable.                                                                  | `ImportantMethod()` | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |
| `ATrapBase` (Actor)                              | Base Class for how Traps interact with Environment and Snapping Features.                                        | `ImportantMethod()` | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |
| `ATimberBuildingComponentBase` (Actor)           | Base Class for Walls and floors. Handles the Complex Snapping utilizing Snap Points (Scene Components)           | `ImportantMethod()` | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |
| `UBuildingComponentPanel` (UI)                   | The UI for selecting a Buildable to Place and get all Buildable info, like synergies and costs.                  | `ImportantMethod()` | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |
| `UBuildingComponent`  (UI)                       | The UI for selecting a Buildable to Place and get all Buildable info, like synergies and costs.                  | `ImportantMethod()` | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |
| `UBuildComponentDataAsset`  (UI)                 | Data Asset used for Loading specific buildable information into the Building Component Panel. |  | [Code](https://github.com/you/yourrepo/blob/main/Source/MyGame/Path/File.h) |

---

## 🗂 Architecture

- **Mermaid Diagram** Showing Build Flow.