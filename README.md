# FlyingBirds

<p align="center">
  <img src="images/FlyingBirds.gif" />
</p>

FlyingBirds is a client-server game that challenges user to code an **autonomous AI** for your own bird.

## Table of Contents

1. [Game Highlights](#game-highlights)  
2. [How It Works](#how-it-works)  
3. [Game Entities](#game-entities)  
4. [Writing Your AI (Lua)](#writing-your-ai-lua)  
5. [Getting Started](#getting-started)  
   1. [Starting the Server](#starting-the-server)  
   2. [Connecting the Client (GUI)](#connecting-the-client-gui)  
6. [Screenshots](#screenshots)  
7. [Contribute](#contribute)  
8. [License](#license)

---

## Game Highlights

- **Control System**: Program your bird’s behavior using a Lua script.
- **Competitive Gameplay**: Absorb smaller players, avoid larger birds, and grab power-ups to stay alive.  
- **Power-Ups**: Power-Ups like Food, SpeedBoost, and Protection spawn around the map to help you grow stronger or survive.
- **Easy to Learn, Fun to Master**: The simple concept (move and grow) hides a deep strategic layer via your custom code.

---

## How It Works

1. **Connect to the Server**  
   - Run or join a server, choose your nickname, and connect with the proper IP/Port.
<p align="center">
  <img src="images/Lobby.png" />
</p>

2. **Your Bird’s AI**  
   - Each game “tick”, you control your bird’s movement using Lua script by returning an angle (in degrees `0–360`) from a function called `getAngle(myId, players, entities)`.
   - Use `players` and `entities` tables to plan strategy:  
     - `players[id]` has fields like `.x`, `.y`, `.mass`, `.protection`, etc.  
     - `entities[id]` has fields like `.x`, `.y`, `.mass`, `.type` (e.g., `"food"`, `"speedboost"`, `"protection"`).
   - The server updates everyone’s positions and handles collisions.

Example of the simple strategy, seek the nearest entity:
```lua
    function getAngle(myId, players, entities)
        local myPlayer = players[myId]
        if not myPlayer then
            return 0
        end

        -- Example strategy: Seek the nearest food/power-up
        local myX, myY = myPlayer.x, myPlayer.y
        local closestId, closestDist2 = nil, math.huge

        for id, e in pairs(entities) do
            if e.type == "food" or e.type == "speedboost" or e.type == "protection" then
                local dx, dy = e.x - myX, e.y - myY
                local dist2 = dx*dx + dy*dy
                if dist2 < closestDist2 then
                    closestDist2 = dist2
                    closestId = id
                end
            end
        end

        if not closestId then
            return 0
        end

        local target = entities[closestId]
        local dx, dy = target.x - myX, target.y - myY
        local angleRadians = math.atan2(dy, dx)
        local angleDegrees = math.deg(angleRadians)

        return angleDegrees
    end
```

Try adding extra logic—like chasing smaller players or avoiding bigger ones—to make your AI more competitive!


3. **Growth and Survival**  
   - Collect **Food** to increase mass.  
   - Pick up **SpeedBoost** to temporarily move faster.  
   - Grab **Protection** to avoid being eaten by larger birds.  
   - *Watch out for bigger birds trying to absorb you!*
<p align="center">
  <img src="images/InGame.png" />
</p>

4. **Victory Conditions**  
   - Survive until time runs out, or be the last bird standing.  
   - Larger mass = more points on the final scoreboard.
<p align="center">
  <img src="images/AfterTheGame.png" />
</p>

---

## Game Entities

### Players (Birds)
- **Position (x, y)**  
- **Mass**: Your size/health—grow by eating food or smaller birds.  
- **Speed**: Movement speed (boostable).  
- **Protection**: Temporary shield from being devoured.  
- **Attacking**: Indicates absorbing a smaller bird.

### Food
- **Static** collectible item.  
- **Increases mass** when eaten.

### SpeedBoost
- **Temporary speed multiplier** to gain an advantage in an escape or chaising.

### Protection
- **Shield** effect preventing absorption for a limited duration.

---

## Getting Started

### 1. Starting the Server

1. Build the project (see instructions below).  
2. From the build folder (or wherever the binary is located), run:
   ```bash
   ./FlyingBirdsServer
3. The server will open a console window and begin waiting for clients to connect. You can see log messages that indicate the server status.

### 1. Connecting the Client (GUI)
Run the Client after building, for example:

./FlyingBirdsClient

A window appears with the main menu. You can enter:

    Server IP: e.g., 127.0.0.1 for localhost.
    Port: Must match the server’s port (default could be 12345, for example).
    Nickname: Any name you’d like to appear in-game.

Click the Play or Connect button. If the connection is successful, you’ll be placed in the lobby state—waiting for the game to start.
Once enough players are connected (or the server admin starts the match), you’ll see your bird in the game world.
Your AI script (the .lua file) is automatically loaded by the client and used every game “tick” to control your movement.
