Greg = {}

function Greg.update(npc)
	
end

function Greg.interact(interact)
	npc = interact:get_entity():get_npc()
	current_line = npc:get_current_line()

	if current_line == 0 then
		print_blank("Old Man: I've spent many years exploring these lands")
		print_blank("Old Man: let me teach you what I've learned.")
	end
end