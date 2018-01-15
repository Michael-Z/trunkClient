-- �û�ģ��
local UserModel = class("UserModel")

function UserModel:ctor()
	self.mUserId = 0									
	self.mHeadId = 0									
	self.mUserLv = 0									
	self.mUserExp = 0								
	self.mGold = 0										
	self.mDiamond = 0									
	self.mTowerCoin = 0									
	self.mPvpCoin = 0									
	self.mUnionContrib = 0								
    self.mFlashCard10 = 0
	self.mFlashCard = 0									
	self.mBagCapacity = 0					
	self.mFreeHeroTimes = 0								

	self.mChangeNameFree = 0							
	self.mResetTimeStamp = 0							
	self.mBuyGoldTimes = 0									

	self.mUnionId = 0									
	self.mApplyUnionStamp = 0							
	self.mPayment = 0							
	self.mMonthCardStamp = 0							

	self.mTotalSignSucCount = 0								
	self.mMonthSignDay = 0								
	self.mTotalSignDay = 0								
	self.mDaySignFlag = 0								

	self.mFirstPayFlag = 0								
	self.mFoundStartFlag = 0							

	self.mUserName = ""		

								

	self.mApplyUnionInfos = {}					
end

function UserModel:init(buffData)
	self.mUserId = buffData:readInt()					-- ���id
	self.mHeadId = buffData:readInt()					-- ���ͷ��
	self.mUserExp = buffData:readInt()					-- ��Ҿ���
	self.mGold = buffData:readInt()						-- ��ҽ��
	self.mDiamond = buffData:readInt()					-- �����ʯ
	self.mTowerCoin = buffData:readInt()				-- ����
	self.mPvpCoin = buffData:readInt()					-- pvp��
	self.mUnionContrib = buffData:readInt()				-- ���ṱ��
    self.mFlashCard10 = buffData:readInt()				-- 10�鿨ȯ
	self.mFlashCard = buffData:readInt()				-- �鿨ȯ
	self.mResetTimeStamp = buffData:readInt()			-- �ָ�ʱ���(������ҹ������, �����������, �����ָ�)
	self.mBagCapacity = buffData:readUShort()			-- �ֱ�������
	self.mUserLv = buffData:readUChar()					-- ��ҵȼ�
	self.mFreeHeroTimes = buffData:readUChar()			-- ��ѳ鿨����
	self.mChangeNameFree = buffData:readUChar()			-- 0Ϊ���,1Ϊ��Ҫ����
	self.mBuyGoldTimes = buffData:readUShort()			-- �����Ҵ���
	self.mPayment = buffData:readInt()
	self.mMonthCardStamp = buffData:readInt()
	self.mTotalSignDay = buffData:readInt()				-- �ۼ�ǩ������
	self.mMonthSignDay = buffData:readInt()				-- �����ۼ�ǩ������
	self.mTotalSignSucCount = buffData:readInt()		-- �Ѿ���ȡ�Ľ׶ν�������

	self.mFoundStartFlag = buffData:readInt()			-- ����ʼ���
	
	self.mDaySignFlag = buffData:readUChar()			-- �Ƿ��Ѿ�ǩ����0-δǩ����1-�Ѿ�ǩ��
	self.mFirstPayFlag = buffData:readUChar()			-- �׳���
	self.mBDType = buffData:readUChar()					-- ��������
	self.mBDLv = buffData:readUChar()					-- ����ȼ�
	
	self.mBDTimeStamp = buffData:readInt()				-- ����ʱ���
	self.mUserName = buffData:readCharArray(32)			-- �û���

	print("mUserId", self.mUserId)
	print("mUserName", self.mUserName, type(self.mUserName))
	print("mHeadId", self.mHeadId)
	print("mUserLv", self.mUserLv)
	return true
end

function UserModel:setUserID(id)
	self.mUserId = id
end

function UserModel:getUserID()
	return self.mUserId
end

function UserModel:setHeadID(id)
	self.mHeadId = id
end

function UserModel:getHeadID()
	return self.mHeadId
end

function UserModel:setGold(num)
	self.mGold = num
end
    
function UserModel:addGold(num)
	self.mGold = self.mGold + num
end

function UserModel:getGold()
	return self.mGold
end

function UserModel:setUserLevel(lv)
	self.mUserLv = lv
end

function UserModel:getUserLevel()
	return self.mUserLv
end

function UserModel:setUserExp(exp) 
	self.mUserExp = exp
end
	
function UserModel:getUserExp()
 	return self.mUserExp
end

function UserModel:setDiamond(num) 
	self.mDiamond = num
end

function UserModel:getDiamond()
 	return self.mDiamond
end

function UserModel:setTowerCoin(coin) 
	self.mTowerCoin = coin
end

function UserModel:getTowerCoin() 
	return self.mTowerCoin
end

function UserModel:setPVPCoin(coin) 
	self.mPvpCoin = coin
end

function UserModel:getPVPCoin()
	return self.mPvpCoin
end

function UserModel:setUnionContrib(contrib)
 	self.mUnionContrib = contrib
end	
	
function UserModel:getUnionContrib()
 	return self.mUnionContrib
end
	
function UserModel:setFlashcard(flashcard) 
	self.mFlashCard = flashcard
end

function UserModel:getFlashcard()
	return self.mFlashCard
end

function UserModel:setFlashcard10(flashcard10) 
	self.mFlashCard10 = flashcard10
end

function UserModel:getFlashcard10()
	return self.mFlashCard10
end

function UserModel:setPayment(payment)
	self.mPayment = payment
end

function UserModel:addPayment(payment)
	self.mPayment = self.mPayment + payment
end

function UserModel:getPayment()
	return self.mPayment
end

function UserModel:setMonthCardStamp(time)
 	self.mMonthCardStamp = time
end

function UserModel:getMonthCardStamp()
	return self.mMonthCardStamp
end

function UserModel:setBuyGoldTimes(times)
	self.mBuyGoldTimes = times
end

function UserModel:getBuyGoldTimes()
 	return self.mBuyGoldTimes
end

function UserModel:getUserName()
	return self.mUserName
end
    
function UserModel:setUserName(name)
	self.mUserName = name
end

function UserModel:setChangeNameFree()
	self.mChangeNameFree = 1
end

function UserModel:setFreeHeroTimes(times) 
	self.mFreeHeroTimes = times
end
	
function UserModel:getFreeHeroTimes()
 	return self.mFreeHeroTimes
end

function UserModel:getChangeNameFree() 
	return self.mChangeNameFree 
end

function UserModel:getTotalSignSucCount()
	return self.mTotalSignSucCount
end

function UserModel:setTotalSignSucCount(totalSignSucCount)
 	self.mTotalSignSucCount = totalSignSucCount
end
    
function UserModel:getMonthSignDay()
	return self.mMonthSignDay
end
    
function UserModel:setMonthSignDay(monthSignDay)
	self.mMonthSignDay = monthSignDay
end
    
function UserModel:getTotalSignDay()
	return self.mTotalSignDay
end

function UserModel:setTotalSignDay(totalSignDay)
	self.mTotalSignDay = totalSignDay
end

function UserModel:getDaySignFlag()
	return self.mDaySignFlag
end

function UserModel:setDaySignFlag(flag)
 	self.mDaySignFlag = flag
end

function UserModel:getFirstPayFlag()
	return self.mFirstPayFlag
end

function UserModel:setFirstPayFlag(flag)
	self.mFirstPayFlag = flag
end

function UserModel:getFundStartFlag()
	return self.mFoundStartFlag 
end

function UserModel:setFundStartFlag(flag)
	self.mFoundStartFlag = flag
end

function UserModel:getBDType()
	return self.mBDType
end

function UserModel:setBDType(BDType)
	self.mBDType = BDType
end

function UserModel:getBDLv()
	return self.mBDLv
end

function UserModel:setBDLv(BDLv)
	self.mBDLv = BDLv
end

function UserModel:getBDTimeStamp()
	return self.mBDTimeStamp
end

function UserModel:setBDTimeStamp(BDTimeStamp)
	self.mBDTimeStamp = BDTimeStamp
end

function UserModel:getIdentity()
	--if self.mBDTimeStamp > os.time() then
        return self.mBDType, self.mBDLv
    --end

    --return 0, 0
end

-- ��������
function UserModel:resetUserData()
	self.mBuyEnergyTimes = 0
	self.mBuyGoldTimes = 0
	self.mFreeHeroTimes = 1
end

return UserModel