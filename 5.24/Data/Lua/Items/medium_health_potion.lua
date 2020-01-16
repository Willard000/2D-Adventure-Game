Medium_Health_Potion = {}

function Medium_Health_Potion.use(potion)
	local player = get_player():get_combat()

	player:set_health(player:get_health() + 500)
	potion:delete()
end