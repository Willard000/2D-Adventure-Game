Blueberry = {}

function Blueberry.use(blueberry)
	local player = get_player():get_combat()

	player:set_mana(player:get_mana() + 100)
	blueberry:delete()
end