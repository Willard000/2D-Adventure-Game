Eskimo_Leader = {}

function Eskimo_Leader.update(enemy)
	local in_combat = enemy:get_entity():get_combat():in_combat()

	if(in_combat) then
		local pos = enemy:get_entity():get_position()
		local x = pos:get_x() + pos:get_w() / 2
		local y = pos:get_y() + pos:get_h() / 2
		local player_pos = get_player():get_position()
		local player_x = player_pos:get_x() + player_pos:get_w() / 2
		local player_y = player_pos:get_y() + player_pos:get_h() / 2
		local magic = enemy:get_entity():get_magic()
		magic:cast(player_x, player_y, x, y)
		magic:stop_cast_time()
	end
end

function Eskimo_Leader.death(enemy)

end