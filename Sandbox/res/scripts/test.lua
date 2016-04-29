function printVersion ()
	io.write("Running ", _VERSION, "\n")
end

playSound = function (name, source) 
	s = Sound.new(name, source)
	s:Play()
end

vectTest = function ()
	v1 = vec2.new(10.51, -31)
	v2 = vec2.new(10.1, 2)
	v1:Add(v2);
	io.write("vec2 (", v1:GetX(), ", ", v1:GetY(), ")")
end