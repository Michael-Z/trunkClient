--��ȡ���Է���������·���ݲ�ͨ����ʾ
local BattleAccountHelper = {}

function BattleAccountHelper.BattleAccount(UIWin,UIFail,bufferData)

end

function BattleAccountHelper.toWinnerNormalSettle(stageId, bufferData)
    --���쳣�������Ϣtable
    local resultData = {}
    resultData.stageId = stageId
	resultData.showLayer = "WinLayer"	
    --StageReward ������ؿ��ض���Ϣ
	resultData.star = bufferData:readInt()
	--2�����ǻ������
	resultData.star2Reason = bufferData:readInt()
	resultData.star3Reason = bufferData:readInt()
    --���߸���
    resultData.realItems = {}
    local rewardCount = bufferData:readInt()
    resultData.Gold = 0
    resultData.Exp = 0
    resultData.Diamond = 0
    resultData.TowerCoin = 0
    for i = 1, rewardCount do
        --����ÿ�����ߵ�����
        local id = bufferData:readInt()
        local num = bufferData:readInt()
--        if id == UIAwardHelper.ResourceID.Gold then 
--            resultData.Gold = num
--        elseif id == UIAwardHelper.ResourceID.Energy then
--            resultData.Energy = num
--        elseif id == UIAwardHelper.ResourceID.Exp then
--            resultData.Exp = num
        if id == UIAwardHelper.ResourceID.Diamond then
            resultData.Diamond = num
        elseif id == UIAwardHelper.ResourceID.TowerCoin then
            resultData.TowerCoin = num
        elseif id == UIAwardHelper.ResourceID.UnionContrib then
            resultData.UnionContrib = num
        elseif id == UIAwardHelper.ResourceID.PvpCoin then
            resultData.PvpCoin = num
        else
            table.insert(resultData.realItems, {id = id, num = num})
        end
    end

    resultData.rewardCountType = "Multiple"
    return resultData
end

function BattleAccountHelper.toLoserNormalSettle()
    local resultData = {}
    resultData.showLayer = "LoseLayer"
    return resultData
end

return BattleAccountHelper



