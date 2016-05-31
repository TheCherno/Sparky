function printVersion ()
	io.write("Running ", _VERSION, "\n")
end

loadSound = function (name, source) 
	s = Sound(name, source)
	SoundManager.Add(s);
end

playSound = function (name) 
	SoundManager.Get(name):Play()
end

loopSound = function (name) 
	SoundManager.Get(name):Loop()
end

changeGain = function (gain) 
	s = SoundManager.Get(name)
end