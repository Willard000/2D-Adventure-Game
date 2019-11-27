Ladder = {}

function Ladder.interact(ladder)
	local t = {} -- t[0]=id t[1]=x t[2]=y t[3]=w t[4]=h

	local k = 0
	for i in string.gmatch(ladder:get_data(), "=(%w*)%S*") do
		table.insert(t, k, i) 
		k = k + 1
	end

	print("Ladder ", ladder:get_data())

	teleport_to(t[0], t[1], t[2], t[3], t[4])
end