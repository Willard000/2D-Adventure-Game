require "math"

Spell_Rocks = {}

function Spell_Rocks.update(spell) 
	spell:death()
end

function Spell_Rocks.cast(spell, x, y, x2, y2)
	local caster = spell:get_caster()
	local pos = spell:get_entity():get_position()
	local caster_pos = caster:get_position()
	pos:set(x2 - pos:get_w() / 2, y2 - pos:get_h() / 2)

	-- direction vec
	local vec = {x = x - pos:get_x() - (pos:get_w() / 2),
		     y = y - pos:get_y() - (pos:get_h() / 2)}

	-- normalize & scale vec
	local vec_mag = 1 / math.sqrt((vec.x^2) + (vec.y^2))
	vec.x = vec.x * vec_mag * spell:get_max_dis()
	vec.y = vec.y * vec_mag * spell:get_max_dis()

	pos:set((x2 - pos:get_w() / 2) + vec.x, (y2 - pos:get_h() / 2) + vec.y)

-- Rotation
	local u = {x = x - pos:get_x() - (pos:get_w() / 2),
		   y = y - pos:get_y() - (pos:get_h() / 2)}
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

	pos:set_rotation(angle)
end