function exitToWindows(element)
	GameManager:getSingletonPtr():requestShutdown()
end

function selectprofile(element)
	itemid = element:getAttribute("itemid")
	ProfileManagerState:getSingletonPtr():selectProfile(itemid)
end

function newprofile(element)
	ProfileManagerState:getSingletonPtr():newProfile()
end