Will = {}

function Will.update(npc)
	
end

function Will.interact(interact)
	npc = interact:get_entity():get_npc()
	current_line = npc:get_current_line()
	player = get_player():get_player()

	if current_line == 0 then
		print_blank("Will: Help me please!")
		print_blank("Will: My scarecrows have turned on me!")
		if not player:has_quest(1) then
			npc:set_current_line(5)
		elseif player:has_quest(1) then
			npc:set_current_line(1)
		end
	elseif current_line == 1 then
		print_blank(player:get_quest_info(1) .. " Scarecrows")
		if player:quest_should_complete(1) and not player:quest_is_complete(1) then
			print_blank("Will: Thank you for killing those scarecrows for me!")
			print_blank("Will: Here is the hat I promised.")
			player:add_item(15)
			player:complete_quest(1)
			npc:set_current_line(2)
		elseif player:quest_is_complete(1) then
			npc:set_current_line(2)
		end
	elseif current_line == 2 then
		print_blank("Will: Thank you for killing those Scarecrows for me!")
	elseif current_line == 5 then
		print_blank("Will: Could you destroy the scarecrows around my farm?")
		npc:set_current_line(6)
	elseif current_line == 6 then
		print_blank("Will: Destroy 10 of them and I will give you my hat.")
		player:activate_quest(1)
		npc:set_current_line(1)
	end
end