Large_Health_Potion = {}

function Large_Health_Potion.use(potion)
	local player = get_player():get_combat()

	player:set_health(player:get_health() + 800)
	potion:delete()
end