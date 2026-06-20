# Qt Chess Game (Jeu d'Échecs)

A chess game built with **C++** and **Qt Widgets**, featuring two modes: **Player vs Player** and **Player vs Machine (basic AI)**.  
The game includes an interactive 8x8 board, move validation, and king-safety checks.

## Features
- Two game modes
  - Player vs Player
  - Player vs Machine (AI plays Black)
- Interactive UI
  - Menu screen
  - Turn indicator (White / Black)
  - Click-to-select piece, then click destination
  - Highlighting for selected square and possible moves
  - New game button
- Chess logic
  - Pawn, Rook, Knight, Bishop, Queen, King moves
  - Prevents capturing your own pieces
  - King safety validation (moves that leave your king in check are rejected)
- AI (simple)
  - Generates legal moves for Black
  - Prioritizes capture moves when available
  - Otherwise plays a random legal move
 
<img width="517" height="553" alt="Capture d&#39;écran 2026-06-20 225302" src="https://github.com/user-attachments/assets/3f113a4c-dfbd-44e2-a2d4-5fa59e0e21d7" />
<img width="518" height="557" alt="Capture d&#39;écran 2026-06-20 225337" src="https://github.com/user-attachments/assets/aef22563-7358-483b-838f-6a15adbb689b" />

## Tech Stack
- C++
- Qt Widgets (Qt 5/Qt 6)
- CMake



