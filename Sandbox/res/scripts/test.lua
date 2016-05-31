loadSound = function (name, source) 
	sound = Sound(name, source)
	return sound
end

playSound = function (sound) 
	sound:Play()
end

loopSound = function (sound) 
	sound:Loop()
end

changeGain = function (sound, gain) 
	sound.Gain = sound.Gain + gain
end