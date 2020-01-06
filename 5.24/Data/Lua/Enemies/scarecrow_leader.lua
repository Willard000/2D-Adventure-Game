Scarecrow_Leader = {}

function Scarecrow_Leader.update(enemy)
	local in_combat = enemy:get_entity():get_combat():in_combat()
	local move = enemy:get_move()

	print(move)

	if(in_combat) then
		if move == 1 then
			local pos = enemy:get_entity():get_position()
			local x = pos:get_x() + pos:get_w() / 2
			local y = pos:get_y() + pos:get_h() / 2
			local magic = enemy:get_entity():get_magic()
			local player_position = get_player():get_position()
			local player_x = player_position:get_x() + player_position:get_w() / 2
			local player_y = player_position:get_y() + player_position:get_h() / 2
			magic:cast(player_x, player_y, x, y)
			magic:stop_cast_time()
			enemy:set_move(2)
		elseif move == 2 then
			enemy:set_move(1)
		end
	end
end

function Scarecrow_Leader.death(enemy)

end