local posStart = -20.0
local posOffset = (-posStart) * 2.0 / 7.0
local duration = 0.5
local durationOffset = 0.25

function onGyro(event, x, y, z, timestamp)
end

local function radianToDegree(radian)
	return radian * 57.29577951
end

local function getAngleRadian(v1, v2)
			local dx = v1.y * v2.z - v1.z * v2.y
			local dy = v1.z * v2.x - v1.x * v2.z
			local dz = v1.x * v2.y - v1.y * v2.x
			local MATH_FLOAT_SMALL = 1.0e-37
			local sqrt = math.sqrt(dx * dx + dy * dy + dz * dz)
			local b = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
			local a = math.atan2(sqrt, b) + MATH_FLOAT_SMALL
			return a
end

local old_h = nil
local old_m = nil
local old_s = nil

local function timeChanged(h, m, s)
	if (h == old_h and m == old_m and s == old_s) then
		return false
	end
	return true
end

function OnTimeTick(event)
	local scene = cc.Director:getInstance():getRunningScene()
	local layer = scene:getChildByName("HelloWorld")

	local sprite = layer:getChildByName("earth")
	local action = cc.RotateBy:create(1.0, cc.vec3(0.0, -2.0, 0.0))
	sprite:runAction(action)

	local sprite = layer:getChildByName("cloud")
	local action = cc.RotateBy:create(1.0, cc.vec3(0.0, -4.0, 0.0))
	sprite:runAction(action)

	local function SetNumbers(name, pos, cameraPos)
		local sprite = layer:getChildByName(name)
		sprite:setVisible(true)
		sprite:setPosition3D(pos)
		sprite:setScale(0.13)

		local v1 = cc.vec3(pos.x - cameraPos.x, pos.y - cameraPos.y, pos.z - cameraPos.z)
		local v2 = cc.vec3(1.0, 0.0, 0.0)
		

		local angleRadian = getAngleRadian(v1, v2)
		local angle = radianToDegree(angleRadian)
		local rot = sprite:getRotation3D()
		rot.y = angle
		sprite:setRotation3D(rot)
		return sprite
	end

	local h = cc.Watch:get_hour24(event)
	local m = cc.Watch:get_minute(event)
	local s = cc.Watch:get_second(event)

	local h1 = math.floor(h / 10)
	local h2 = h % 10
	local m1 = math.floor(m / 10)
	local m2 = m % 10
	local s1 = math.floor(s / 10)
	local s2 = s % 10

	print("OnTimeTick : ", h1, h2, ":", m1, m2, ":", s1, s2)

	for i = 0, 9 do
		local name = string.format("num_s2_%d", i)
		local sprite = layer:getChildByName(name)
		sprite:setVisible(false)
	end

	local camera = layer:getChildByName("camera")
	local cameraPosition = camera:getPosition3D()

	local name = string.format("num_s2_%d", s2)
	local sprite = SetNumbers(name, cc.vec3(20.0, 0.0, 120.0), cameraPosition)

	local action = cc.RotateBy:create(1, { x = 0.0, y = -180.0, z = 0.0})
	sprite:runAction(action)

		local function animateFinishedCallBack()
--			for i = 0, 9 do
--				local name = string.format("num_s2_%d", i)
--				local sprite = layer:getChildByName(name)
--				sprite:setVisible(false)
--			end
		end

	if old_h == nil then
			local name = string.format("num_h1_%d", h1)
			local sprite = SetNumbers(name, cc.vec3(posStart, 0.0, 120.0), cameraPosition)		
			local action = cc.RotateBy:create(duration + durationOffset * 6, { x = 0.0, y = -90.0, z = 0.0})
			sprite:runAction(action)

			local name = string.format("num_h2_%d", h2)
			local sprite = SetNumbers(name, cc.vec3(posStart + posOffset, 0.0, 120.0), cameraPosition)		
			local action = cc.RotateBy:create(duration + durationOffset * 5, { x = 0.0, y = -90.0, z = 0.0})
			sprite:runAction(action)

			local name = string.format("num_h_semi")
			local sprite = SetNumbers(name, cc.vec3(posStart + posOffset * 2, 0.0, 120.0), cameraPosition)		
			local action = cc.RotateBy:create(duration + durationOffset * 4, { x = 0.0, y = -90.0, z = 0.0})
			sprite:runAction(action)

			local name = string.format("num_m1_%d", m1)
			local sprite = SetNumbers(name, cc.vec3(posStart + posOffset * 3, 0.0, 120.0), cameraPosition)		
			local action = cc.RotateBy:create(duration + durationOffset * 3, { x = 0.0, y = -90.0, z = 0.0})
			sprite:runAction(action)

			local name = string.format("num_m2_%d", m2)
			local sprite = SetNumbers(name, cc.vec3(posStart + posOffset * 4, 0.0, 120.0), cameraPosition)		
			local action = cc.RotateBy:create(duration + durationOffset * 2, { x = 0.0, y = -90.0, z = 0.0})
			sprite:runAction(action)

			local name = string.format("num_m_semi")
			local sprite = SetNumbers(name, cc.vec3(posStart + posOffset * 5, 0.0, 120.0), cameraPosition)		
			local action = cc.RotateBy:create(duration + durationOffset, { x = 0.0, y = -90.0, z = 0.0})
			sprite:runAction(action)

			local name = string.format("num_s1_%d", s1)
			local sprite = SetNumbers(name, cc.vec3(posStart + posOffset * 6, 0.0, 120.0), cameraPosition)		
			local action = cc.RotateBy:create(duration, { x = 0.0, y = -90.0, z = 0.0})
			sprite:runAction(action)
	else
		if (timeChanged(h, m, s)) then

			local function disable(sender)
				sender:setVisible(false)
				print("disable")
			end

			local function enableS1(sender)
				local name = string.format("num_s1_%d", s1)
				local pos = sender:getPosition3D()
				local sprite = SetNumbers(name, pos, cameraPosition)
				local action = cc.RotateBy:create(duration, { x = 0.0, y = -90.0, z = 0.0})
				sprite:runAction(action)
			end

			local function enableM2(sender)
				local name = string.format("num_m2_%d", m2)
				local pos = sender:getPosition3D()
				local sprite = SetNumbers(name, pos, cameraPosition)
				local action = cc.RotateBy:create(duration, { x = 0.0, y = -90.0, z = 0.0})
				sprite:runAction(action)
			end

			local function enableM1(sender)
				local name = string.format("num_m1_%d", m1)
				local pos = sender:getPosition3D()
				local sprite = SetNumbers(name, pos, cameraPosition)
				local action = cc.RotateBy:create(duration, { x = 0.0, y = -90.0, z = 0.0})
				sprite:runAction(action)
			end

			local function enableH2(sender)
				local name = string.format("num_h2_%d", h2)
				local pos = sender:getPosition3D()
				local sprite = SetNumbers(name, pos, cameraPosition)
				local action = cc.RotateBy:create(duration, { x = 0.0, y = -90.0, z = 0.0})
				sprite:runAction(action)
			end

			local function enableH1(sender)
				local name = string.format("num_h1_%d", h1)
				local pos = sender:getPosition3D()
				local sprite = SetNumbers(name, pos, cameraPosition)
				local action = cc.RotateBy:create(duration, { x = 0.0, y = -90.0, z = 0.0})
				sprite:runAction(action)
			end

			local old_h1 = math.floor(old_h / 10)
			local old_h2 = old_h % 10
			local old_m1 = math.floor(old_m / 10)
			local old_m2 = old_m % 10
			local old_s1 = math.floor(old_s / 10)
			local old_s2 = old_s % 10

			if (old_s1 ~= s1) then
				local oldName = string.format("num_s1_%d", old_s1)
				local oldSprite = layer:getChildByName(oldName)
				local rotate = cc.RotateBy:create(duration, { x = 0.0, y = -90.0, z = 0.0})
				local seq = cc.Sequence:create(rotate, cc.CallFunc:create(disable), cc.CallFunc:create(enableS1))
				oldSprite:runAction(seq)
			end

			if (old_m2 ~= m2) then
				local oldName = string.format("num_m2_%d", old_m2)
				local oldSprite = layer:getChildByName(oldName)
				local rotate = cc.RotateBy:create(duration + durationOffset, { x = 0.0, y = -90.0, z = 0.0})
				local seq = cc.Sequence:create(rotate, cc.CallFunc:create(disable), cc.CallFunc:create(enableM2))
				oldSprite:runAction(seq)
			end

			if (old_m1 ~= m1) then
				local oldName = string.format("num_m1_%d", old_m1)
				local oldSprite = layer:getChildByName(oldName)
				enableName = string.format("num_m1_%d", m1)
				local rotate = cc.RotateBy:create(duration + durationOffset * 2, { x = 0.0, y = -90.0, z = 0.0})
				local seq = cc.Sequence:create(rotate, cc.CallFunc:create(disable), cc.CallFunc:create(enableM1))
				oldSprite:runAction(seq)
			end

			if (old_h2 ~= h2) then
				local oldName = string.format("num_h2_%d", old_h2)
				local oldSprite = layer:getChildByName(oldName)
				enableName = string.format("num_h2_0%d", h2)
				local rotate = cc.RotateBy:create(duration + durationOffset * 3, { x = 0.0, y = -90.0, z = 0.0})
				local seq = cc.Sequence:create(rotate, cc.CallFunc:create(disable), cc.CallFunc:create(enableH2))
				oldSprite:runAction(seq)
			end

			if (old_h1 ~= h1) then
				local oldName = string.format("num_h1_%d", old_h1)
				local oldSprite = layer:getChildByName(oldName)
				enableName = string.format("num_h1_%d", h1)
				local rotate = cc.RotateBy:create(duration + durationOffset * 4, { x = 0.0, y = -90.0, z = 0.0})
				local seq = cc.Sequence:create(rotate, cc.CallFunc:create(disable), cc.CallFunc:create(enableH1))
				oldSprite:runAction(seq)
			end

		end
	end

	old_h = h
	old_m = m
	old_s = s

--[[
	local seq = cc.Sequence:create(action, cc.CallFunc:create(animateFinishedCallBack))
	sprite:runAction(seq)
	local sprite = SetNumbers("num_h_0", cc.vec3(0.0, 0.0, 120.0), cc.vec3(0.0, 0.0, 120.0))
	local action = cc.RotateBy:create(1.0, cc.vec3(0.0, -90.0, 0.0))
	sprite:runAction(action)
--]]

end

function OnAmbientTick(event)
	print("OnAmbientTick")
end

function OnAmbientChanged(event)
	print("OnAmbientChanged")
end

function onTouchBegan(touch, event)
	print("onTouchBegan")
end

