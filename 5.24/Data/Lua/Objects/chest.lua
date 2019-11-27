Chest = {}

function Chest.interact(chest)
	position = chest:get_entity():get_position()
	x = position:get_x() + position:get_w() / 2
	y = position:get_y() + position:get_h() / 2
	
	local t = {} -- t[0]=type [1]=type_id

	local k = 0
	for i in string.gmatch(chest:get_data(), "=(%w*)%S*") do
		table.insert(t, k, i)
		k = k + 1
	end
	
	create_entity(t[0], t[1], x, y)
	chest:get_entity():destroy()
end