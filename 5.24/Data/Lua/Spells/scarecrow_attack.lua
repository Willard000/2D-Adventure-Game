require "math"

Scarecrow_Attack = {}

function Scarecrow_Attack.update(spell) 
	spell:move(spell:get_dx(), spell:get_dy())

	if(spell:get_dis() > spell:get_max_dis()) then
		spell:death()
	end
end

function Scarecrow_Attack.cast(spell, x, y)
	local caster = spell:get_caster()
	local pos = spell:get_entity():get_position()
	local caster_pos = caster:get_position()

	pos:set(caster_pos:get_x() + (caster_pos:get_w() / 2) - (pos:get_w() / 2),
		caster_pos:get_y() + (caster_pos:get_h() / 2) - (pos:get_h() / 2))

	-- direction vec
	local vec = {x = x - pos:get_x() - (pos:get_w() / 2),
		     y = y - pos:get_y() - (pos:get_h() / 2)}

	-- normalize & scale vec
	local vec_mag = 1 / math.sqrt((vec.x^2) + (vec.y^2))
	vec.x = vec.x * vec_mag * spell:get_speed()
	vec.y = vec.y * vec_mag * spell:get_speed()

	spell:set_dx(vec.x)
	spell:set_dy(vec.y)
end