# New GBA project - using ECS architecture
Run the .gba file in an emulator or on an actual Game Boy Advance.
# Objective
Power meter depletes over time.  
Collecting one of the firefly/moth things will increment this meter a bit...  
But leaving any behind before the whole group disappears will completely deplete your power meter.  
The closer you are to the enemy when the firefly is picked up, the more power is bestowed. Take risks!  
If your health is fully depleted by the enemy, the game soft resets and the points are reset to zero.  
*ADVANCED:* If you collect a whole group of fireflies without pressing a direction, your power fills to maximum instantly.  
When power is full, it stops depleting and the player can then defeat the enemy to gain one point and a bit of health back.  
Collect ten points to win.  
# Controls
- Dpad to move
- Start to pause game
- Hold Select and press Start to reset game
#### DEBUG FEATURES (Not enabled by default):
- B to spawn a bunch of entities
- L or R to turn on hitbox visualiser, then:
  - L to make hitbox smaller on x axis, R to make hitbox bigger on x axis
  - Hold A while pressing L or R for y axis
- Select to toggle gravity
# Progress
### :) DONE:  
- Implemented basic ECS architecture
- Finished sprite and tile data allocation methods
- Made a pretty fun game
- Implemented basic UI

### :| TODO:
- Implement better UI
- Add "game over" and "you win" screens
- Change the background image from Melee Island to something else
# ECS architecture
I started making this game to explore the benefits of a non-object oriented system for a game engine. Entity Component System describes an
architecture where game entities are made up of individual components stored contiguously in separate arrays and connected via a common
entity ID. This has advantages for cache hit ratios and, in my opinion, for code extensibility later down the line. Adding/removing
components from an entity makes it easy to greatly change its behaviour without too much refactoring.  
The benefits wrt cache hits basically don't exist on Game Boy Advance (it has no CPU cache to speak of). I mainly chose GBA for its ease
of use and coolness factor. The platform presents some interesting challenges; no arithmetic acceleration hardware of any kind makes
division (except by constants that are powers of 2) and floating-point maths very slow. No branch prediction hardware means that branching is quite expensive and should be avoided.
# Resources used
[Tonc GBA Programming Guide](https://gbadev.net/tonc/)  
[Libtonc](https://github.com/devkitPro/libtonc)  
gbadev Discord  
[Game Boy Advance Resource Management article by Rafael Baptista](https://www.gamedeveloper.com/programming/gameboy-advance-resource-management)  
[Game Programming Patterns by Robert Nystrom, specifically the Component chapter](https://gameprogrammingpatterns.com/component.html)  
[Usenti GBA graphics editor](https://www.coranac.com/projects/usenti/)  
