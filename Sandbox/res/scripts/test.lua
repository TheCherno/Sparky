function printVersion ()
	io.write("Running ", _VERSION, "\n")
end

playSound = function () 
	s = Sound.new("cherno", "res/Cherno.ogg")
	s:Play()
end
