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
	
	-- direction
	local vec = {x = get_mouse_x() - spell:get_x() - (spell:get_w() / 2),
		     y = get_mouse_y() - spell:get_y() - (spell:get_h() / 2)}

	local abs_vec = {x = math.abs(vec.x), y = math.abs(vec.y)}

	local dx = 0
	local dy = 0

	if(abs_vec.y > abs_vec.x) then
		dx = abs_vec.x / abs_vec.y
		dy = 2 - dx
	else
		dy = abs_vec.y / abs_vec.x
		dx = 2 - dy
	end

	if(vec.x < 0) then
		dx = 0 - dx
	end
	
	if(vec.y < 0) then
		dy = 0 - dy
	end

	dx = dx * spell:get_speed()
	dy = dy * spell:get_speed()

	spell:set_dx(dx)
	spell:set_dy(dy)

-- Rotation

	local u = {x = get_mouse_x() - spell:get_x(), y = get_mouse_y() - spell:get_y()}
	local v = {x = 1, y = 0}

	local dot_product = (u.x * v.x) + (u.y * v.y)
	
	local u_mag = math.sqrt((u.x^2) + (u.y^2))
	local v_mag = math.sqrt((v.x^2) + (v.y^2))

	local mag = u_mag * v_mag

	local angle = math.acos(dot_product / mag)

	angle = angle * (180 / math.pi)

	if(u.y < 0) then
		angle = 360 - angle
	end

	spell:set_rotation(angle)
end

cast()
