# New GBA project - using ECS architecture
Run the .gba file in a GBA emulator or on GBA hardware. Right now there's basically no gameplay. Fun will be added in future  
# Controls:
- Dpad to move
- B to spawn a bunch of entities
- L or R to turn on hitbox visualiser, then:
  - L to make hitbox smaller on x axis, R to make hitbox bigger on x axis
  - Hold A while pressing L or R for y axis
- Start to reset game
- Select to toggle gravity
# Progress
DONE:  
- Implemented basic ECS architecture
- Finished sprite and tile data allocation methods

TODO:
- Make fun
- Implement health, UI, fail state
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
