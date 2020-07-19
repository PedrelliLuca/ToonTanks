# ToonTanks

Third game made by following the [Unreal C++ Developer course](https://www.gamedev.tv/p/learn-unreal-engine-c-developer-4-22-for-video-game-development?coupon=1D3ABAD8). I loved this section, both because we finally made an actual 3d game and because I learned a lot of things:

- How different classes (Tank, Turret, Projectile) can interact with each other through C++ code
- How `GameMode`s and `PlayerController`s can affect the gameplay
- How blueprints can actually be useful for things like widgets. I already created a widget before, in my [TableAndChairs](https://github.com/PedrelliLuca/TableAndChairs) project, but now I have a better understanding of them and blueprints in general.

The gameplay is very simple: the player controls a tank which can be moved with WASD, aim with the cursor, and fire with the left mouse button. In order to win the game, the player has to destroy all of the turrets before before they destroy the tank.

## Personal touches

These are some additions I made to the base course game.

### PawnPatrolTurret

Turret that moves along a line repeatedly, reversing its motion when one end is reached, similar to what Crash Bandicoot's skunks (and many other enemies in the same series) do.

From the editor it's possible to change the lenght on the line (patrol amplitude) and the speed at which the turret moves (patrol speed). Moreover, the `PawnPatrolTurret` class is a child of `PawnTurret` (class created in the base course), which makes the fire rate and the range customizable too.

The location the turret is placed into the map represents one of its ends, while its rotation defines the line direction; the other end of the patrol line is automatically calculated based on this rotation and the patrol amplitude. 

If the turret collides with something (a wall, the player tank, another turret) it reverses its motion in advance. However, the amplitude doesn't change, which causes the turret to effectively change its extremes of motion along the patrol direction.  
This behavious is intentional: the other option I had was to keep the ends fixed no matter what, which has the disadvantage of having some cycles in which the amplitude is reduced. I don't like this latter option because it seems like a lie to the game designers: if they choose a certain amplitude from the editor, the amplitude should stay the same whenever it's possible.  
If the turret is placed between two obstacles whose distance is less than the patrol amplitude, obviously the amplitude cannot be respected, but this is the only case it might occurr and it is fine.

I had to face many challenges: 
- How is the direction of the patrol line defined? 
- How do I move along a line that isn't one of the axes?
- How can I reverse the movement while keeping the amplitude constant, since the DeltaTime might change every cycle? 
- How do collisions affect the turret's movement?

I had a lot of fun working on this!!

### Scoring system

Now the game has a scoring system: game developers can assign a certain amount of points to each turret class.

Each time a turret is destroyed, its value is added to the player score count. At the end of the game, regardless of whether the player won or lost, a widget shows how many points were collected over the total.

### Health pack

Health packs heal the tank at contact if the current health is less than the starting one. If the tank is at full health, healrh packs can be picked up but they have no effect. The healing amount can be set from the editor.

Fun fact: at their core, health packs work like projectiles! They deal damage on hit, except their damage is negative, i.e. it adds health to the player instead of subtracting it!

### PawnStalkerTurret

Turret that moves towards the player. The AI is very very minimal (for example, the turret does nothing to avoid collisions).