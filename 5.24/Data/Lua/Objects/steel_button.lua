Steel_Button = {}

function Steel_Button.interact(button)
	local gate = interact_find_gate(button:get_data(), "Steel_Bars")
	
	if gate then
		gate:destroy()
	end
end