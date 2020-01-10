Bed = {}

function Bed.interact(bed)
	reload_all()
	stats = get_player():get_combat()
	stats:set_health(stats:get_max_health())
	stats:set_mana(stats:get_max_mana())
	print_blank("You feel well rested.")
end