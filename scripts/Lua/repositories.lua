function load()
	RepositoryManager:getSingletonPtr():loadFactions("factions.txt")
	RepositoryManager:getSingletonPtr():loadHubs("hubs.txt")
	RepositoryManager:getSingletonPtr():loadFactionHubs("factionhubs.txt")
	RepositoryManager:getSingletonPtr():loadBattlefields("battlefields.txt")
	RepositoryManager:getSingletonPtr():loadTerrainTextures("textures.txt")
	RepositoryManager:getSingletonPtr():loadTerrains("terrains.txt")
	RepositoryManager:getSingletonPtr():loadRandomMissionTypes("randommissiontypes.txt")
	RepositoryManager:getSingletonPtr():loadRanks("ranks.txt")
	RepositoryManager:getSingletonPtr():loadSounds("sounds.txt")
	
	RepositoryManager:getSingletonPtr():loadArmors("armor.txt")
	RepositoryManager:getSingletonPtr():loadBatteries("batteries.txt")
	RepositoryManager:getSingletonPtr():loadDrives("drives.txt")
	RepositoryManager:getSingletonPtr():loadReactors("reactors.txt")
	RepositoryManager:getSingletonPtr():loadSensors("sensors.txt")
	RepositoryManager:getSingletonPtr():loadShields("shields.txt")
	RepositoryManager:getSingletonPtr():loadTargetingSystems("targetingsystems.txt")
	RepositoryManager:getSingletonPtr():loadBeamProjectiles("beams.txt")
	RepositoryManager:getSingletonPtr():loadPhysicalProjectiles("physicalprojectiles.txt")
	RepositoryManager:getSingletonPtr():loadWeapons("weapons.txt")
	
	
	RepositoryManager:getSingletonPtr():loadMCUs("mcus.txt")
	RepositoryManager:getSingletonPtr():loadWeaponHardpoints("hardpoints.txt")
	RepositoryManager:getSingletonPtr():loadAnimations("animations.txt")
	RepositoryManager:getSingletonPtr():loadWeaponAnimations("weaponanimations.txt")
	RepositoryManager:getSingletonPtr():loadAIMCUs("aimcus.txt")
	RepositoryManager:getSingletonPtr():loadAIGroups("aigroups.txt")
	
	
end