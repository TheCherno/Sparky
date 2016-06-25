local sound = nil

loadSound = function (name, source)
	sound = Sound(name, source)
	SoundManager.Add(sound)
end

playSound = function (name)
	sound = SoundManager.Get(name)
	sound:Stop()
	sound:Play()
end

loopSound = function (s)
	s:Loop()
end

changeGain = function (name, gain)
	sound = SoundManager.Get(name)
	g = sound:GetGain() + gain;
	sound:SetGain(g)
end