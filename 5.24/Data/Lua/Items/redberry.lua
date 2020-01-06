Redberry = {}

function Redberry.use(redberry)
	local player = get_player():get_combat()

	player:set_health(player:get_health() + 100)
	redberry:delete()
end