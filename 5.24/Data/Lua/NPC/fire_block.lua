Fire_Block = {}

function Fire_Block.update(npc)
	npc:get_entity():get_sprite():set_ani(3)

	local pos = npc:get_entity():get_position()
	local player_pos = get_player():get_position()
	if in_range(pos, player_pos, 10) then
		local dis = 100
		if pos:get_x() + 1 >= player_pos:get_x() + player_pos:get_w() then
			pos:move_dis(4, dis)
		elseif pos:get_y() + 1 >= player_pos:get_y() + player_pos:get_h() then
			pos:move_dis(2, dis)
		elseif pos:get_x() + pos:get_w() <= player_pos:get_x() + 1 then
			pos:move_dis(3, dis)
		elseif pos:get_y() + pos:get_h() <= player_pos:get_y() + 1 then
			pos:move_dis(1, dis)
		end
		
	end

	snow_block = get_nearby_entity(pos, 15, 2, 35)
	if snow_block then
		snow_block:destroy()
	end
end