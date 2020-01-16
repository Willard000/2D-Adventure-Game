Small_Health_Potion = {}

function Small_Health_Potion.use(potion)
	local player = get_player():get_combat()

	player:set_health(player:get_health() + 300)
	potion:delete()
end