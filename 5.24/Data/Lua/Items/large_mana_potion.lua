Large_Mana_Potion = {}

function Large_Mana_Potion.use(potion)
	local player = get_player():get_combat()

	player:set_mana(player:get_mana() + 800)
	potion:delete()
end