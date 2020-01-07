require "math"

local function split(spell)
	if not spell:get_caster_deleted() then
	local spell_pos = spell:get_entity():get_position()
	local enemy_pos = get_player():get_position()
	local x = spell_pos:get_x()
	local y = spell_pos:get_y()
	local magic = spell:get_caster():get_magic()
	magic:cast_special(x, y + 10000, x, y, 3)
	magic:cast_special(x, y - 10000, x, y, 3)
	magic:cast_special(x + 10000, y, x, y, 3)
	magic:cast_special(x - 10000, y, x, y, 3)
	magic:cast_special(x + 10000, y + 10000, x, y, 3)
	magic:cast_special(x + 10000, y - 10000, x, y, 3)
	magic:cast_special(x - 10000, y + 10000, x, y, 3)
	magic:cast_special(x - 10000, y - 10000, x, y, 3)
	end
end

Scarecrow_Leader_Attack = {}

function Scarecrow_Leader_Attack.update(spell) 
	spell:move(spell:get_dx(), spell:get_dy())
	spell:get_entity():get_position():rotate(5.0)

	if(spell:get_dis() > spell:get_max_dis()) then
		split(spell)
		spell:death()
	end
end

function Scarecrow_Leader_Attack.cast(spell, x, y, x2, y2)
	local caster = spell:get_caster()
	local pos = spell:get_entity():get_position()
	local caster_pos = caster:get_position()
	pos:set(x2 - pos:get_w() / 2, y2 - pos:get_h() / 2)

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