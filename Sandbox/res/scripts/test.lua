printVersion = function ()
	io.write("Running ", _VERSION, "\n")
end

loadSound = function (name, source) 
	s = audio.Sound(name, source)
	audio.SoundManager.Add(s);
end

playSound = function (name) 
	audio.SoundManager.Get(name):Play()
end

loopSound = function (name) 
	audio.SoundManager.Get(name):Loop()
end

changeGain = function (gain) 
	s = audio.SoundManager.Get(name)
	s:Gain(s:Gain + gain)
end