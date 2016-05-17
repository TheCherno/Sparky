function printVersion ()
	io.write("Running ", _VERSION, "\n")
end

local Scene = nil

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
	s:SetGain(s:GetGain() + gain)
end

add = function (vp, m, scene)
	e = Entity()
	e:AddComponent(TransformComponent(vp))
	e:AddComponent(MeshComponent(m))
	scene:Add(e)
end

scene = function (g)
	g:print()
end