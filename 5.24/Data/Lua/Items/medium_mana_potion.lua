Medium_Mana_Potion = {}

function Medium_Mana_Potion.use(potion)
	local player = get_player():get_combat()

	player:set_mana(player:get_mana() + 500)
	potion:delete()
end