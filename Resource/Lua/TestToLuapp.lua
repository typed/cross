require("Lua/log_tree")
require("Lua/extern")
print("This is TestToLuapp.")
local node = Node.New()
node:Trace()

--[[
LcCharLua = class("LcCharLua", function() return Node.New() end)
function LcCharLua:ctor()
    self.m_lcValue = 1
    print("LcCharLua:ctor")
end

local c = LcCharLua.new()
root:AddChild(c)
]]