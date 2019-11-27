Bandit_Leader = {}

function Bandit_Leader.update(enemy)
	in_combat = enemy:get_entity():get_combat():in_combat()

	if(in_combat) then
		pos = enemy:get_entity():get_position()
		x = pos:get_x()
		y = pos:get_y()
		magic = enemy:get_entity():get_magic()
		magic:cast(x, y + 10000)
		magic:cast(x, y - 10000)
		magic:cast(x + 10000, y)
		magic:cast(x - 10000, y)
		magic:cast(x + 10000, y + 10000)
		magic:cast(x + 10000, y - 10000)
		magic:cast(x - 10000, y + 10000)
		magic:cast(x - 10000, y - 10000)
		magic:stop_cast_time()
	end
end