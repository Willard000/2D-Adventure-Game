Scarecrow = {}

function Scarecrow.update(enemy)
	local in_combat = enemy:get_entity():get_combat():in_combat()

	if(in_combat) then
		local pos = enemy:get_entity():get_position()
		local x = pos:get_x() + pos:get_w() / 2
		local y = pos:get_y() + pos:get_h() / 2
		local magic = enemy:get_entity():get_magic()
		magic:cast(x, y + 10000, x, y)
		magic:cast(x, y - 10000, x, y)
		magic:cast(x + 10000, y, x, y)
		magic:cast(x - 10000, y, x, y)
		magic:cast(x + 10000, y + 10000, x, y)
		magic:cast(x + 10000, y - 10000, x, y)
		magic:cast(x - 10000, y + 10000, x, y)
		magic:cast(x - 10000, y - 10000, x, y)
		magic:stop_cast_time()
	end
end

function Scarecrow.death(enemy)
	local player = get_player():get_player()

	if player:has_quest(1) then
		player:add_kill_count(1)
	end
end