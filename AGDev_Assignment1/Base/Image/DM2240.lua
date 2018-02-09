function UpdateLineInFile(inputString, fileName, lineNumber)
	local file = assert(io.open(fileName, "rb"))
	local sContents = file:read("*all")
	local linesArray = {}
	local value = 1
	for line in io.lines(fileName) do
		if value == lineNumber then
			linesArray[#linesArray + 1] = inputString
		else
			linesArray[#linesArray + 1] = line
		end
		value = value + 1
	end
		file = assert(io.open(fileName, "w+"))
		for i = 1, #linesArray do
			file:write(linesArray[i])
			file:write("\n")
		end
		file:close()
end

function SaveToLuaFile(outputString, fileName, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open(fileName, "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open(fileName, "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)

	print(distanceSquare)
	return distanceSquare
end

function GetMinMax(...)
	local minValue = -1
	local maxValue = -1
	local avgValue = -1
	local numValues = 0
	for i,v in ipairs(arg) do
		if minValue == -1 then
			minValue = v
		elseif v < minValue then
			minValue = v
		end

		if maxValue == -1 then
			maxValue = v
		elseif v > maxValue then
			maxValue = v
		end
		
		avgValue = avgValue + v
		numValues = numValues + 1
    end
	avgValue = avgValue / numValues
	print(minValue, maxValue, avgValue, numValues)
	return minValue, maxValue, avgValue, numValues
end

function ReplaceForward(newValue)
	moveForward = string.char(newValue)
	print(string.char(newValue))
	
end

function CompareScore(score1, score2, score3)
	if score1 >= score2 then
		highscore = score1
	else
		highscore = score2
	end
	
	if score3 > highscore then
		highscore = score3
	end
	return highscore;
end

title = "DM2240 - Week 14 Scripting"
width = 1024
height = 768

-- Keyboard Inputs
moveForward  = "W"
moveBackward = "S"
moveLeft     = "A"
moveRight    = "D"

-- CPlayerInfo start position
CPlayerInfoStartPos = {0,0,10}

--Waypoints
Waypoint_A_1 = {x=30.0, y=0.0, z=0.0}
Waypoint_A_2 = {x=100.0, y=0.0, z=100.0}
Waypoint_A_3 = {x=-30.0, y=0.0, z=0.0}



--print(moveForward)
