Small_Mana_Potion = {}

function Small_Mana_Potion.use(potion)
	local player = get_player():get_combat()

	player:set_mana(player:get_mana() + 300)
	potion:delete()
end