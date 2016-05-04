function printVersion ()
	io.write("Running ", _VERSION, "\n")
end

s = 0;

playSound = function (name, source) 
	s = Sound(name, source)
	s:Play()
end

loopSound = function (name, source) 
	s = Sound(name, source)
	s:Loop()
end

changeGain = function (gain) 
	s:SetGain(s:GetGain() + gain)
	io.write("Gain ", s:GetGain())
end