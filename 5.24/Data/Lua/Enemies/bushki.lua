Bushki = {}

function Bushki.update(enemy)
	in_combat = enemy:get_entity():get_combat():in_combat()

	if(in_combat) then
		player_pos = get_player():get_position()
		x = player_pos:get_x() + player_pos:get_w() / 2
		y = player_pos:get_y() + player_pos:get_h() / 2
		enemy:get_entity():get_magic():cast(x, y)
	end
end