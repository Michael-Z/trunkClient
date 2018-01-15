
local UIReconnect = class("UIReconnect", function ()
	return require("common.UIView").new()
end)

--����״̬, �ص���Ӧ�ķ���
local RoomState = {
    "noRoom",         --���䲻����
    "prepareLoad",    --�ȴ���ҽ��뷿��
    "loading",        --������
    "idleTime",       --����׼��ʱ��
    "fighting",       --ս����
    "gameover"        --��Ϸ����
}

function UIReconnect:ctor()
    --��ʼ������
    --self.rootPath = ResConfig.UIReconnect.Csb2.mainPanel
    --self.root = getResManager():getCsbNode(self.rootPath)
    --self.rootAct = cc.CSLoader:createTimeline(self.rootPath)
    --self.root:runAction(self.rootAct)
    --self:addChild(self.root)
end

function UIReconnect:onOpen()
    --ע������ص�
    local reconnectCmd = NetHelper.makeCommand(MainProtocol.Pvp, PvpProtocol.ReconnectSC)
    self.callbackHandler = handler(self, self.onReconnectCallback)
    NetHelper.setResponeHandler(reconnectCmd, self.callbackHandler)
    --������������
    local pvpModel = getGameModel():getPvpModel()
    local battleId = pvpModel:getPvpInfo().BattleId
    local BufferData = NetHelper.createBufferData(MainProtocol.Pvp, PvpProtocol.ReconnectCS)
    BufferData:writeInt(battleId)
    NetHelper.request(BufferData)
end

function UIReconnect:onClose()
    --ȡ������ص�
    local reconnectCmd = NetHelper.makeCommand(MainProtocol.Pvp, PvpProtocol.ReconnectSC)
    NetHelper.removeResponeHandler(reconnectCmd, self.callbackHandler)
end 

function UIReconnect:onReconnectCallback(main, sub, data)
    print("UIReconnect callback!!")
    --���ݷ��صķ���״̬, ѡ���Ӧ�ص�
    local state = data:readInt()
    local roomType = data:readInt()
    getGameModel():getPvpModel():setRoomType(roomType)
    local func = self[RoomState[state]]
    print("call"..RoomState[state] .." ".. state)
    if func then
        func()
    end
end

function UIReconnect:noRoom()
    --����battleidΪ0
    local pvpModel = getGameModel():getPvpModel()
    pvpModel:setBattleId(0)
    --�лش�������
    SceneManager.loadScene(SceneManager.Scene.SceneHall)
end

function UIReconnect:prepareLoad()
    --�е�ƥ�����
    UIManager.open(UIManager.UI.UIArenaMatch, 2)
end

function UIReconnect:loading()
    --�е����ؽ��� 
    UIManager.open(UIManager.UI.UIArenaMatch, 3)
end

function UIReconnect:fighting()
     --�е����ؽ��� 
    UIManager.open(UIManager.UI.UIArenaMatch, 4)
end

function UIReconnect:gameover()
    --����battleidΪ0
    local pvpModel = getGameModel():getPvpModel()
    pvpModel:setBattleId(0)
    --�лش�������
    SceneManager.loadScene(SceneManager.Scene.SceneHall)
end

return UIReconnect
