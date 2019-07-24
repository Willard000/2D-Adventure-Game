Ripple = {}

function Ripple.update(effect)
	effect:get_entity():get_sprite():set_ani(3)
	effect:get_entity():get_position():rotate(.001)
end