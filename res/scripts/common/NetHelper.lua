--[[
	NetHelper��Ҫ���ڴ�����������������Ӧ�������쳣������򵥽������ʹ�á�
	1.��η�������
		����NetHelper.createBufferData����������������+������ο�Э�飩��
		�÷����᷵��һ��BufferData���󣬵��øö����writeInt��writeString�ȷ���д�����ݣ�API�ο�LuaBufferData.cpp��
		����Ҫ���͵����ݺ󣬵���NetHelper.request����������BufferData���󣨿���Զ���������ֶΣ�
	2.��μ������������ص���Ӧ
		�������һ���ص���NetHelper�У�ʾ�����£�
			ע��һ�������������Ӧ�ص�
			local cmd = NetHelper.makeCommand(CMD_HALL, CMD_HALL_BUY_COIN_SC)
			NetHelper.setResponeHandler(cmd, onHallBuyCoinSC)
		C++���������ڽ��յ��������Ϣʱ��ص�NetHelper.onResponse������������BufferData����
		�ڻص������д���ͻ��˵���Ӧ�߼�
		������Ҫ�ûص�ʱ��Ӧ����NetHelper.removeResponeHandler���ûص��Ƴ�
		
	3.���ڵ������ԣ�������request�����У������ʱ���룬������˷��صĽ������ֱ�ӵ���onResponse
	4.���������쳣�ȵĴ�������...
	
	2015-8-27 by ��ү
]]


NetHelper = NetHelper or {}
NetHelper.callbacks = {}

-- ����Э���������ûص�
-- �ص����������б� ���� ����������BufferData����
-- BufferData�����Ѿ�ƫ�Ƶ���ͷ������ݲ���
function NetHelper.setResponeHandler(cmd, callback)
	NetHelper.callbacks[cmd] = { [callback] = true }
end

-- ����Э��������ӻص�
-- �ص����������б� ���� ����������BufferData����
-- BufferData�����Ѿ�ƫ�Ƶ���ͷ������ݲ���
function NetHelper.addResponeHandler(cmd, callback)
	if NetHelper.callbacks[cmd] == nil then
		-- ��ʼ��table
		NetHelper.callbacks[cmd] = { [callback] = true }
	else
		NetHelper.callbacks[cmd][callback] = true
	end
end

-- �Ƴ��ص�������ֻ��cmd����
function NetHelper.removeResponeHandler(cmd, callback)
    print("removeResponeHandler:" .. cmd)
	if callback and NetHelper.callbacks[cmd] ~= nil then
		NetHelper.callbacks[cmd][callback] = nil
	else
		NetHelper.callbacks[cmd] = nil
	end
end

-- �������Ӧʱ�ص��÷���
function NetHelper.onResponse(bufferData)
	local head = NetHelper.getHead(bufferData)
    if nil == head then
        return
    end

	local cmd = head.cmd
    print("receive network cmd" .. cmd)
	local funs = NetHelper.callbacks[cmd]
	if funs ~= nil then
		for fun,_ in pairs(funs) do
			print("NetHelper.onResponse, cmd, func, bufferData", head.maincmd, head.subcmd, bufferData)
			fun(head.maincmd, head.subcmd, bufferData)
            head = NetHelper.getHead(bufferData)
            if nil == head then
                break
            end
		end
	end
end

--����bufferData���ذ�ͷ����һ��Table��
function NetHelper.getHead(bufferData)
	--�����õ���ͷλ���ٶ�ȡ��ͷ
	bufferData:resetOffset()
	local ret = {}
	ret.length = bufferData:readInt()
	ret.cmd = bufferData:readInt()
	ret.id = bufferData:readInt()
    if "number" ~= type(ret.cmd) then
        print("data type is error!!!")
        return nil
    end

	--����C�ṩ�Ļ�ȡ�������������ӿ�
	ret.maincmd = getMainCmd(ret.cmd)
	ret.subcmd = getSubCmd(ret.cmd)
	return ret
end

-- �����������������ϳ�һ������
function NetHelper.makeCommand(mainCmd, subCmd)
	-- makeCommand��C++��װ��ȫ�ֺ���
	return makeCommand(mainCmd, subCmd)
end

-- ������������������һ���Ѿ�������ͷ��Buf����
function NetHelper.createBufferData(mainCmd, subCmd)
	-- newBufferData��C++��װ��ȫ�ֺ���
	bufferData = newBufferData()
	-- length�ֶ� ��C++�������ֱ�ӻ�ȡbuf�ĳ��Ƚ����޸�
	bufferData:writeInt(0)
	-- cmd�ֶ�
	bufferData:writeInt(makeCommand(mainCmd, subCmd))
	-- id�ֶ� �Կͻ��˶��Դ��ֶ�����
	bufferData:writeInt(0)
	return bufferData
end

-- ����BufferData��������ͷţ���ÿ��request֮�󶼻��Զ��ͷ�
-- ��������BufferData�������û��requestʱ����Ҫ���ø÷��������ͷţ��Ա����ڴ�й¶
function NetHelper.deleteBufferData(bufferData)
	-- newBufferData��C++��װ��ȫ�ֺ���
	deleteBufferData(bufferData)
end

-- ֱ�ӷ�����������,connTypeΪ����ķ���������0session,1chat
function NetHelper.request(bufferData, connType)
	-- request��C++��װ��ȫ�ֺ���
    if connType then
        request(bufferData, connType)
    else
        request(bufferData)
    end
	
	-- deleteBufferData��C++��װ��ȫ�ֺ���
	deleteBufferData(bufferData)
end

-- �����������󣬵ȴ�ָ����Ӧ�����ε��
-- ��ʾ����ȴ�����������ȴ���ʾ�ӳ�ʱ�䡿
-- ���糬ʱ����ʾ�������
function NetHelper.requestWithTimeOut(bufferData, s2cCmd, callback, tipsTime, timeOut)
    tipsTime = tipsTime or 1.0
    timeOut = timeOut or 10.0

    local responeCallback = function (mainCmd, subCmd, data)
        if callback then
            callback(mainCmd, subCmd, data)
        end

        NetworkTips.closeTips()
    end

    -- �����Ӧ����(ע����ӵ�����������)
    NetHelper.addResponeHandler(s2cCmd, responeCallback)
    
    request(bufferData)
    NetworkTips.createTips(tipsTime, timeOut, bufferData, s2cCmd, responeCallback)
end

function NetHelper.setErrorHandler(callback)
	-- ���������쳣�Ļص�
end

return NetHelper