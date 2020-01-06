Plant = {}

function Plant.interact(plant)
	local player = get_player():get_player()
	
	player:add_item(plant:get_data())
	plant:get_entity():destroy()
end