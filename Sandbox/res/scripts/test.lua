printVersion = function ()
	io.write("Running ", _VERSION, "\n")
end

s = nil

loadSound = function (name, source) 
	s = audio.Sound(name, source)
	audio.SoundManager.Add(s);
end

playSound = function (name) 
	s = audio.SoundManager.Get(name)
	s:Play()
end

loopSound = function (name) 
	s = audio.SoundManager.Get(name)
	s:Loop()
end

changeGain = function (sound, gain) 
	sound.Gain = sound.Gain + gain
end