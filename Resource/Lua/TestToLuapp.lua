require("Lua/log_tree")
require("Lua/extern")
print("This is TestToLuapp.")

local root = GetRootNode()

local luaUserData = {
    name = "hello",
}
print("SetLuaUserData:"..tostring(luaUserData))
root:SetLuaUserData(luaUserData)
local luaUserData1 = root:GetLuaUserData()
print("GetLuaUserData:"..tostring(luaUserData1))


local node = Node.New()
root:AddChild(node)
node:SetName("aaa")

local char = Char.New()
root:AddChild(char)
char:SetName("bbb")

local player = Player.New()
root:AddChild(player)
player:SetName("ccc")
player:SetAge(3)
player:SetControl(3)
--print("Control="..player:GetControl())
--print("Age="..player:GetAge())

--root:Trace()
--player:RemoveFromParent()
--root:Trace()
--player:Trace()
--print(tostring(player))


PlayerLua = class("PlayerLua", function() return Player.New() end)
function PlayerLua:ctor()
    print("PlayerLua:ctor")
    self:SetName("ddd")
    self.m_value = 99
    self.CppMethod_Trace = GetCppSuperClassMethod(self, "Trace")
end
--重写C++方法
function PlayerLua:Trace()
    self:CppMethod_Trace()
    print("PlayerLua Value="..self.m_value)
end

MainPlayerLua = class("MainPlayerLua", PlayerLua)
function MainPlayerLua:ctor()
    self.super.ctor(self)
    print("MainPlayerLua:ctor")
    self:SetName("eee")
    self.m_mainvalue = 9900
end
function MainPlayerLua:Trace()
    self.super.Trace(self)
    print("MainPlayerLua MainValue="..self.m_mainvalue)
end

local pl = MainPlayerLua.new()
pl:SetAge(1000)
pl:SetControl(2000)
root:AddChild(pl)

local mpl = root:FindChildByName("eee")
print(tostring(pl))
print(tostring(mpl))
mpl:Trace()

mpl:RemoveFromParent()
root:Trace()