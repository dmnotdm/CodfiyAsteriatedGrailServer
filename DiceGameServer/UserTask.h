#ifndef __USER_TASK_H__
#define __USER_TASK_H__

#include <time.h>
#include "Config.h"
#include "zMisc.h"
#include "zTCPTask.h"
#include "MessageQueue.h"
#include "GameManager.h"
#include "GameGrail.h"

using namespace std;
class GameGrail;
class UserTask : public zTCPTask, public MessageQueue
{
private:      
	string		m_userId;
	int         m_playerId;
	int         m_tableId;
	bool        m_bAuthen;		//�Ƿ���ָ��ʱ���ڷ��͵�һ��Login��,���û���ߵ�
	time_t      m_activeTime;
	int32_t     m_iCheckTime;
	int32_t		m_gameType;

	uint32_t	m_iTmpId;
	static uint32_t m_sIDSeq;

public:
	UserTask( boost::asio::io_service& service):zTCPTask(service) 
	{
//FIXME used to mimic userId, which should be retrieved from DB
		m_userId = "";
		m_playerId = -1;
		m_bAuthen = false;
		m_activeTime = time(NULL);
		m_iCheckTime = ServerConfig::getInstance().m_iCheckTime;
	}

	virtual ~UserTask()
	{
		ztLoggerWrite(ZONE,e_Debug,"~UserTask ,[%s] close",m_userId.c_str() );
	}
	string getUserID(){  return m_userId;  }
	bool msgParse(const void *pstrMsg, const uint32_t nCmdLen);
	bool cmdMsgParse(const char *pstrMsg, const uint32_t nCmdLen);
	void setPlayerID(int id) { m_playerId = id; } 
	GameGrail* getGame();
	bool tryNotify(int id, int state, int step = 0, void* reply = NULL);
	void Start();
	void OnQuit();
	void OnCheck();

private:
	bool handleUserLogin(const char *pstrCmd, const uint32_t nCmdLen);
	bool handleGetGameList(const char *pstrCmd, const uint32_t nCmdLen);
	bool handleSicBoSitIntoTable(const char *pstrCmd, const uint32_t nCmdLen);
	bool handleSicBoGameBetAction(const char *pstrCmd, const uint32_t nCmdLen);
	bool handleSicBoGameRefresh(const char *pstrCmd, const uint32_t nCmdLen);
};

#endif