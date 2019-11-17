Steel_Button = {}

function Steel_Button.interact(button)
	gate = interact_find_gate(button:get_special(), "Steel_Bars")
	
	if gate then
		gate:destroy()
	end
	
	print("Button_Channel: ", button:get_special())
end