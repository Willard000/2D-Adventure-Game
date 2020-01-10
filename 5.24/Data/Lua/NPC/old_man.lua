Old_Man = {}

function Old_Man.update(npc)
	
end

function Old_Man.interact(interact)
	npc = interact:get_entity():get_npc()
	current_line = npc:get_current_line()

	if current_line == 0 then
		print_blank("Old Man: I've spent many years exploring these lands,")
		print_blank("Old Man: let me teach you what I've learned.")
		npc:set_current_line(1)
	elseif current_line == 1 then
		print_blank("Old Man: Move around using WASD.")
		npc:set_current_line(2)
	elseif current_line == 2 then
		print_blank("Old Man: Attack using mouse buttons.")
		npc:set_current_line(3)
	elseif current_line == 3 then
		print_blank("Old Man: Interact with the E key.")
		print_blank("Old Man: Open your inventory with the F key.")
		print_blank("Old Man: Pick up items with the Q key.")
		npc:set_current_line(4)
	elseif current_line == 4 then
		print_blank("Old Man: Your progress will be automatically be saved.")
		npc:set_current_line(5)
	elseif current_line == 5 then
		print_blank("Old Man: Sleeping will restore your stats, but")
		print_blank("Old Man: will also respawn all enemies and objects.")
		print_blank("Old Man: Good Luck!")
		npc:set_current_line(0)
	end
end