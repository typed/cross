require("Lua/log_tree")
require("Lua/extern")
print("This is TestLuaBridge.")
local root = GetRootNode()
local node = Node.New()
node:SetName("Node1")
root:AddChild(node)
local char = Char.New()
char:SetAge(80)
char:SetName("Char1")
root:AddChild(char)
local ply = Player.New()
ply:SetAge(11)
ply:SetControl(100)
ply:SetName("Player1")
root:AddChild(ply)

char:RemoveFromParent()

root:Trace()

LcCharLua = class("LcCharLua", function() return Node.New() end)
function LcCharLua:ctor()
    self.m_lcValue = 1
    print("LcCharLua:ctor")
end

local c = LcCharLua.new()
root:AddChild(c)