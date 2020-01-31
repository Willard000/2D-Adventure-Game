Greg = {}

function Greg.update(npc)
	
end

function Greg.interact(interact)
	npc = interact:get_entity():get_npc()
	current_line = npc:get_current_line()
	player = get_player():get_player()

	if current_line == 0 then
		print_blank("Greg: I have built a wall of rocks around my house")
		print_blank("Greg: to defend me against those creatures.")
		if not player:has_quest(0) then
			npc:set_current_line(5)
		elseif player:has_quest(0) then
			npc:set_current_line(1)
		end
	elseif current_line == 1 then
		print_blank(player:get_quest_info(0) .. " Bushkis")
		if player:quest_should_complete(0) and not player:quest_is_complete(0) then
			print_blank("Greg: Thank you for killing those nasty creatures for me!")
			print_blank("Greg: Here take this shirt I made.")
			player:add_item(7)
			player:complete_quest(0)
			npc:set_current_line(2)
		elseif player:quest_is_complete(0) then
			npc:set_current_line(2)
		end
	elseif current_line == 2 then
		print_blank("Greg: Thank you for killing those nasty creatures for me!")
	elseif current_line == 5 then
		print_blank("Greg: Could you get rid of some of them for me?")
		npc:set_current_line(6)
	elseif current_line == 6 then
		print_blank("Greg: Yes kill 10 of those bush creatures and")
		print_blank("Greg: I will reward you with some new clothes")
		player:activate_quest(0)
		npc:set_current_line(1)
	end
end