Scarecrow_Leader = {}

local function first_cast(enemy)
	local magic = enemy:get_entity():get_magic()
	local pos = enemy:get_entity():get_position()
	local x = pos:get_x() + pos:get_w() / 2
	local y = pos:get_y() + pos:get_h() / 2
	local player_position = get_player():get_position()
	local player_x = player_position:get_x() + player_position:get_w() / 2
	local player_y = player_position:get_y() + player_position:get_h() / 2
	magic:cast(player_x, player_y, x, y)
	magic:stop_cast_time()
end

local function second_cast(enemy)
	local magic = enemy:get_entity():get_magic()
	local pos = enemy:get_entity():get_position()
	local x = pos:get_x() + pos:get_w() / 2
	local y = pos:get_y() + pos:get_h() / 2
	magic:cast_second(x, y + 10000, x, y)
	magic:cast_second(x, y - 10000, x, y)
	magic:cast_second(x + 10000, y, x, y)
	magic:cast_second(x - 10000, y, x, y)
	magic:cast_second(x + 10000, y + 10000, x, y)
	magic:cast_second(x + 10000, y - 10000, x, y)
	magic:cast_second(x - 10000, y + 10000, x, y)
	magic:cast_second(x - 10000, y - 10000, x, y)
	magic:stop_cast_time()
end

function Scarecrow_Leader.update(enemy)
	local in_combat = enemy:get_entity():get_combat():in_combat()
	local move = enemy:get_move()
	local magic = enemy:get_entity():get_magic()
	local can_cast = magic:get_can_cast()
	
	if(in_combat) then
		if can_cast and move < 3 then
			first_cast(enemy)
			enemy:set_move(move + 1)
		elseif can_cast and move >= 3 then
			second_cast(enemy)
			enemy:set_move(1)
		end
	end
end

function Scarecrow_Leader.death(enemy)

end