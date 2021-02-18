# The Ultimate RPG

This is the final build of the RPG created for the PS Advanced C++ of WS2020/21.

## Necessary Libraries

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- Cereal

## Controls

- wasd        -> move around, explore the world
- i           -> open inventory
- esc         -> open menu
- o           -> save game
- p           -> save game into new save file
- konami code -> walk through walls and over water!

## Combat

The combat utilizes a cooldown based system. Inital cooldowns are decided using the agility stat of all combatants. When the cooldown of a combatant reaches 0, the combatant is able use an attack or ability, which in turn sets a new cooldown depending on the attack or ability. If both an enemy and a player combatant reach a cooldown of 0 at the same time, the player combatant is allowed to attack first. A combat encounter is won when all enemies are dead.

Other important concepts:

- Blocking: A combatant that is blocking cannot be damaged using an attack. When the blocking combatant is hit, the block is broken and the combatant can be damaged again. Using attacks while blocking is theoretically possible. Entering blocking state while burning inflicts burn damage, but also cleanses the burning state.

- Burning: A combatant that is burning takes a certain amount of damage when the state resets to normal or a block is performed to cleanse the burn. Additionally, if a burning combatant takes damage from an attack, additional burn damage is inflicted and the state reset cooldown is slightly increased. Burning combatants are not influenced by their state when using attacks or abilities.

- State Reset: When a combatant enters a state that is neither normal or dead, a state reset cooldown is started. When the state reset cooldown reaches 0, the state is reset to normal and potential additional effects are applied.

- Slow: Taking damage by a slowing attack increases the cooldown of the defender.

- Armor-Breaking: Some attacks in this game have an armor-breaking effect. When calculating the damage of such attack, potential bonus defence offered by the defenders equipment is ignored.

## Inventory

Manage your equipment by drag-and-dropping the desired items onto one of your combatants itemslots. Each combatant can carry up to 4 items, which influence your abilities and stats during combat.
By dragging an item to the red framed pocket, the item will be permanently deleted.

## Combatants

Combatants are characters that fight for your party during combat. Each of the 3 combatants has different starting stats that decide their inital strength, weaknesses and roles. Increase the stats by leveling up your combatants in combat.

- Knight (55 HP, 9 AGI, 8 STR, 6 DEF, 11 DEX, 7 PER, 7 Luck)
- Archer (45 HP, 11 AGI, 3 STR, 8 DEF, 3 DEX, 13 PER, 12 Luck)
- Brute (75 HP, 2 AGI, 15 STR, 11 DEF, 8 DEX, 2 PER, 7 Luck) 

## Stats

Each combatant and enemy has different stats that influence all facets of combat.

- HP: The HP of a combatant denotes the damage needed to defeat them during the next attack. Each combatant and enemy always starts combat at an inital HP value (which can be increased by leveling up). The HP of a combatant in combat can exceed this inital HP value.
- Agility (AGI): At the start of each combate encounter, the inital cooldowns of all combatants are set using their agilities. The maximum inital cooldown is 10.
- Strength (STR): When a melee attack hits its target, the ratio of the strength of the attacker and the defense of the defender is used to influence the damage of the attack.
- Defence (DEF): Additionally to the function noted in the description of strength, the damage of ranged attacks is influenced by the ration of the penetration of the weapon used to perform the attack and the defence by the defender.
- Dexterity (DEX): When performing a melee attack, the ratio between the dexterity of the attacker and the dexterity of the defender is used to influence the accuracy of the attack.
- Perception (PER): When performing a ranged attack, the ratio between the perception of the attacker and the perception of the defender is used to influence the accuracy of the attack.
- Luck: When performing an attack that has a chance to critically hit, the ratio between the luck of the attacker and the luck of the defender is used to influence the chance to achieve such a critical hit.

## Guide

The text guide in the guide folder contains additional information about attacks, weapons and enemies.

## Credits

Sound effects obtained from https://www.zapsplat.com
Assets obtained from https://craftpix.net/ and https://kenney.nl
Map tileset from https://pipoya.itch.io

Framework inspired by Carl Birch https://www.youtube.com/user/creaper
