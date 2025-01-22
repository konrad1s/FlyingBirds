-- user_algo.lua

function getAngle(myId, players, entities)
    local myPlayer = players[myId]
    if not myPlayer then
        return 0
    end

    local myX = myPlayer.x
    local myY = myPlayer.y

    local closestId = nil
    local closestDist2 = math.huge

    -- find closest food
    for id, e in pairs(entities) do
        if e.type == "food" then
            local dx = e.x - myX
            local dy = e.y - myY
            local dist2 = dx*dx + dy*dy
            if dist2 < closestDist2 then
                closestDist2 = dist2
                closestId = id
            end
        end
    end

    if not closestId then
        return 0
    end

    local target = entities[closestId]
    local dx = target.x - myX
    local dy = target.y - myY

    local angleRadians = math.atan(dy, dx)
    local angleDegrees = math.deg(angleRadians)

    return angleDegrees
end
