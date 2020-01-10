Chest = {}

-- interact_data = typeid=x_id=y
function Chest.interact(chest)
	local position = chest:get_entity():get_position()
	local x = position:get_x() + position:get_w() / 2
	local y = position:get_y() + position:get_h() / 2
	
	local t = {} -- t[0]=type [1]=type_id

	local k = 0
	for i in string.gmatch(chest:get_data(), "=(%w*)%S*") do
		table.insert(t, k, i)
		k = k + 1
	end
	
	create_entity(t[0], t[1], x, y)
	chest:get_entity():destroy()
end