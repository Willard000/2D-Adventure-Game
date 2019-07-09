require "math"

function update(spell)
	spell:move(spell:get_dx(), spell:get_dy())

	if(spell:get_dis() > spell:get_max_dis()) then
		spell:death()
	end

end

local function cast()
	local spell = SPELL_CASTED

	spell:set_x(spell:get_owner():get_x() + spell:get_owner():get_w() / 2)
	spell:set_y(spell:get_owner():get_y() + spell:get_owner():get_h() / 2)

	-- mouse coordiantes
	local x = get_mouse_x()
	local y = get_mouse_y()
	
	-- adjacent & opposite triangle sides
	local adj = x - spell:get_x() - (spell:get_w() / 2)
	local opp = y - spell:get_y() - (spell:get_h() / 2)
	local abs_adj = math.abs(adj)
	local abs_opp = math.abs(opp)

	-- rise & run
	local dx = 0
	local dy = 0

	if(abs_opp > abs_adj) then
		dx = abs_adj / abs_opp
		dy = 2 - dx
	else
		dy = abs_opp / abs_adj
		dx = 2 - dy
	end

	if(adj < 0) then
		dx = 0 - dx
	end
	
	if(opp < 0) then
		dy = 0 - dy
	end

	dx = dx * spell:get_speed()
	dy = dy * spell:get_speed()

	spell:set_dx(dx)
	spell:set_dy(dy)
end

cast()
