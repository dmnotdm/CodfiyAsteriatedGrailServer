#pragma once
#include "CardEntity.h"
#include "zLogger.h"
#include <boost/lexical_cast.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "base.pb.h"
#include "action_respond.pb.h"

#include <list>

using namespace boost::interprocess;
using namespace network;
using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) { delete (x); (x) = NULL; } }
#endif

enum GrailError{
	GE_SUCCESS,
	GE_TIMEOUT,
	GE_URGENT,
	GE_EMPTY_HANDLE,
	GE_NO_STATE,
	GE_DECK_OVERFLOW,
	GE_CARD_NOT_ENOUGH,
	GE_HANDCARD_NOT_FOUND,
	GE_BASIC_EFFECT_NOT_FOUND,
	GE_BASIC_EFFECT_ALREADY_EXISTS,
	GE_EXCLUSIVE_EFFECT_NOT_FOUND,
	GE_MOVECARD_FAILED,
    GE_INCONSISTENT_STATE,
	GE_FATAL_ERROR,
	GE_INVALID_PLAYERID,
	GE_INVALID_CARDID,
	GE_INVALID_ACTION,
	GE_INVALID_STEP,
	GE_INVALID_EXCLUSIVE_EFFECT,
	GE_NOT_SUPPORTED,
	GE_INVALID_ARGUMENT,
	GE_NO_CONTEXT,
	GE_NO_REPLY
};

enum CAUSE{
	CAUSE_DEFAULT,
	CAUSE_OVERLOAD,
	CAUSE_USE,
	CAUSE_ATTACK,
	CAUSE_POISON,
	CAUSE_WEAKEN,
	CAUSE_MISSILE,
	CAUSE_BUY,
	CAUSE_SYNTHESIZE,
	JI_FENG_JI = 101,
	LIE_FENG_JI = 102,
	LIAN_XU_JI = 103,
	SHENG_JIAN = 104,
	JIAN_YING = 105,
	XUE_YING_KUANG_DAO = 201,
	XUE_XING_PAO_XIAO = 202,
	KUANG_HUA = 203,
	XUE_YING_KUANG_DAO_USED = 204,
	SI_LIE = 205,
	JING_ZHUN_SHE_JI = 301,
	SHAN_GUANG_XIAN_JING = 302,
	JU_JI = 303,
	SHAN_DIAN_JIAN = 304,
	GUAN_CHUAN_SHE_JI = 305,
	FENG_ZHI_FENG_YIN = 401,
	SHUI_ZHI_FENG_YIN = 402,
	HUO_ZHI_FENG_YIN = 403,
	DI_ZHI_FENG_YIN = 404,
	LEI_ZHI_FENG_YIN = 405,
	FA_SHU_JI_DONG = 406,
	WU_XI_SHU_FU = 407,
	FENG_YIN_PO_SUI = 408,
	FAN_SHI = 501,
	SHUI_YING = 502,
	QIAN_XING = 503,
	ZHI_LIAO_SHU = 601,
	ZHI_YU_ZHI_GUANG = 602,
	BING_SHUANG_DAO_YAN = 603,
	LIAN_MIN = 604,
	SHENG_LIAO = 605,
	TIAN_SHI_ZHI_QIANG = 701,
	MO_BAO_CHONG_JI = 801,
	MO_DAN_ZHANG_WO = 802,
	MO_DAN_RONG_HE = 803,
	HUI_MIE_FENG_BAO = 804,
	HUI_YAO = 1001,
    CHENG_JIE = 1002,
    SHENG_GUANG_QI_YU = 1003,
	TIAN_QIANG = 1004,
	DI_QIANG = 1005,
	SHEN_SHENG_XIN_YANG = 1006,
	SHENG_JI = 1007,
	FENG_REN = 1101,
	BING_DONG = 1102,
	HUO_QIU = 1103,
	YUN_SHI = 1104,
	LEI_JI = 1105,
	YUAN_SU_DIAN_RAN = 1106,
	YUE_GUANG = 1107,
	YUAN_SU_XI_SHOU = 1108,
	QI_ZHA = 1201,
	MAO_XIAN_ZHE_TIAN_TANG = 1202,
	TOU_TIAN_HUAN_RI = 1203,
	TE_SHU_JIA_GONG = 1204,
	BU_XIU = 1301,
	SHENG_DU = 1302,
	WEN_YI = 1303,
	SI_WANG_ZHI_CHU = 1304,
	MU_BEI_YUN_LUO = 1305,
	XUN_JIE_CI_FU = 1601,
	WEI_LI_CI_FU = 1602,
	LING_HUN_ZHEN_BAO = 2201,
	LING_HUN_CI_YU = 2202,
	XUE_ZHI_BEI_MING = 2301
};

enum CHANGE{
	CHANGE_ADD,
	CHANGE_REMOVE
};

enum DECK{
	DECK_PILE = 1, 
	DECK_DISCARD = 2,
	DECK_HAND = 4,
	DECK_BASIC_EFFECT = 5,
	DECK_COVER = 6
};

enum HARM_TYPE{
	HARM_NONE,
	HARM_ATTACK,
	HARM_MAGIC
};

enum HIT_RATE{
	RATE_NORMAL,
	RATE_NOREATTACK,
	RATE_NOMISS
};

enum REATTACK{
	RA_ATTACK,
	RA_BLOCK,
	RA_GIVEUP
};

enum SpecialActionId{
	SPECIAL_BUY,
	SPECIAL_SYNTHESIZE,
	SPECIAL_EXTRACT
};

enum STEP{
	STEP_INIT = 0,
	STEP_DONE = 9999
};

#define CARDSUM 150
#define CARDBUF 30
#define MAXPLAYER 8
extern CardEntity* cardList[CARDSUM];
CardEntity* getCardByID(int id);
#define RED  1
#define BLUE 0

//应战行动，注意BLOCKED指圣光和应战，HIT指命中或圣盾（由server判定）
#define REPLYBATTLE  0
#define BLOCKED      1
#define HIT          2

//攻击类别，普通、无法应战、必定命中
#define NORMAL  0
#define NOREPLY 1
#define NOMISS  2

//特殊行动
#define BUY     0
#define SYNTHESIZE 1
#define EXTRACT    2

//通讯协议号
#define COMMONMAGIC 0
#define BEGINNOTICE 2
#define TURNBEGINNOTICE 3
#define ACTIONCOMMAND 4
#define ASKFORREBAT 5
#define REBATCOMMAND 6
#define ASKFORDISCARD 7
#define DISCARDCOMMAND 8
#define DRAWNOTICE 9
#define RESHUFFLE 10
#define MORALENOTICE 11
#define ENDNOTICE 12
#define DISCARDNOTICE 13
#define HITNOTICE 14
#define STONENOTICE 15
#define READYBEGIN 16
#define CUPNOTICE 17
#define ENERGYNOTICE 18
#define MOVECARDNOTICE 19
#define ATTACKHURTNOTICE 20
#define MAGICHURTNOTICE 21
#define ASKFORWEAK 22
#define WEAKCOMMAND 23
#define WEAKNOTICE 24
#define SHIELDNOTICE 25
#define ASKFORMISSILE 26
#define MISSILECOMMAND 27
#define USECARDNOTICE 28
#define ASKFORACTION 29
#define CROSSCHANGENOTICE 32
#define ASKFORCROSS 33
#define ANSFORCROSS 34
#define ASKFORSKILL 35
#define ANSFORSKILL 36
#define CHARACTERNOTICE 37
#define UNACTIONAL 30
#define UNACTIONALNOTICE 31
#define NOTICE 38

typedef struct{	
	int type;
	int point;
	int srcID;
	int cause;
}HARM;

typedef struct{	
	int allowAction;
	int cause;
}ACTION_QUOTA;

#define TOQSTR(x)  boost::lexical_cast<std::string>(x)

string combMessage(string item1,string item2 = "",string item3 = "",string item4 = "",string item5 = "",string item6 = "",string item7 = "");
//编码器,编辑各类通讯信息
class Coder
{
public:
    static string beginNotice(string seatCode){return "2;" + seatCode + ";";}
    static string turnBegineNotice(int ID){return "3;" + TOQSTR(ID) + ";";}
    static void askForReBat(int type,int cardID,int dstID,int srcID, CommandRequest& cmd_req)
	{
		cmd_req.set_cmd_type(CMD_RESPOND);
		Command *cmd;

		cmd = cmd_req.add_commands();
		cmd->set_respond_id(network::RESPOND_REPLY_ATTACK);
		cmd->add_args(type);
		cmd->add_args(cardID);
		cmd->add_args(dstID);
		cmd->add_args(srcID);
	}
    static void askForDiscard(int ID, int sum, int cause, bool show, CommandRequest& cmd_req)
	{
		cmd_req.set_cmd_type(CMD_RESPOND);
		Command *cmd;

		cmd = cmd_req.add_commands();
		cmd->set_respond_id(network::RESPOND_DISCARD);
		cmd->add_dst_ids(ID);
		cmd->add_args(cause);
		cmd->add_args(sum);
		cmd->add_args(show);
	}
	static void handNotice(int ID, list<int> handCards, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);

		list<int>::iterator it;
		for (it = handCards.begin(); it != handCards.end(); ++it)
		{
			player_info->add_hands(*it);
		}
		player_info->set_hand_count(handCards.size());
		if (handCards.size() == 0)
			player_info->add_delete_field("hands");
	}
	static void coverNotice(int ID, list<int> coverCards, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);

		list<int>::iterator it;
		for (it = coverCards.begin(); it != coverCards.end(); ++it)
		{
			player_info->add_covereds(*it);
		}
		player_info->set_covered_count(coverCards.size());
		if (coverCards.size() == 0)
			player_info->add_delete_field("covereds");
	}
	static void basicNotice(int ID, list<BasicEffect> basicCards, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);

		list<BasicEffect>::iterator it;
		for (it = basicCards.begin(); it != basicCards.end(); ++it)
		{
			player_info->add_basic_cards(it->card);
		}
		if (basicCards.size() == 0)
			player_info->add_delete_field("basic_cards");
	}
	static void exclusiveNotice(int ID, bool* exclusive, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);
		bool hasEx = false;
		for (int i = 0; i < EXCLUSIVE_NUM; i++)
		{
			if(exclusive[i]){
				player_info->add_ex_cards(i);
				hasEx = true;
			}
		}
		if (!hasEx)
			player_info->add_delete_field("ex_cards");
	}
    static string askForDiscover(int ID, int sum,string show){return combMessage("49",TOQSTR(ID),TOQSTR(sum),show);}
    static string reshuffleNotice(int howManyNew){return combMessage("10",TOQSTR(howManyNew));}
    static string endNotice(int winColor){return "12;" + TOQSTR(winColor) + ";";}
    static string discardNotice(int ID,int sum,string show,vector < int > cards);
    static void hitNotice(int result,int isActiveAttack,int dstID,int srcID, HitMsg& hit_msg)
	{
		hit_msg.set_cmd_id(isActiveAttack);
		hit_msg.set_hit(result);
		hit_msg.set_src_id(srcID);
		hit_msg.set_dst_id(dstID);
	}
    static void stoneNotice(int color, int gem, int crystal, GameInfo& update_info)
	{
	    if(color == RED){
	        update_info.set_red_gem(gem);
			update_info.set_red_crystal(crystal);
		}
		else{
		    update_info.set_blue_gem(gem);
			update_info.set_blue_crystal(crystal);
		}
	}
    static string cupNotice(int color,int cup){return combMessage("17",TOQSTR(color),TOQSTR(cup));}
    static void energyNotice(int ID, int gem, int crystal, GameInfo& update_info)
	{
		SinglePlayerInfo* player_info = update_info.add_player_infos();
		player_info->set_id(ID);
		player_info->set_gem(gem);
		player_info->set_crystal(crystal);
	}
	static void crossNotice(int ID, int cross, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);
		player_info->set_heal_count(cross);
	}
    static string getCardNotice(int sum,vector < int > cards,int dstID,bool show);
    static void hurtNotice(int dstID, int srcID, int type, int point, int cause, HurtMsg& hurt_msg)
	{
		hurt_msg.set_dst_id(dstID);
		hurt_msg.set_src_id(srcID);
		hurt_msg.set_type(type);
		hurt_msg.set_hurt(point);
		hurt_msg.set_cause(cause);
	}
	static void askForWeak(int ID, int howMany, CommandRequest& cmd_req)
	{
		cmd_req.set_cmd_type(CMD_RESPOND);
		Command *cmd;

		cmd = cmd_req.add_commands();
		cmd->set_respond_id(network::RESPOND_WEAKEN);
		cmd->add_args(ID);
		cmd->add_args(howMany);
	}
    //static string weakNotice(int ID,int act,int howMany=3){return combMessage("24",TOQSTR(ID),TOQSTR(act),TOQSTR(howMany));}
    static string shieldNotic(int ID){return "25;" + TOQSTR(ID) + ";";}
    static void askForMissile(int dstID,int srcID,int hurtSum,int nextID, CommandRequest& cmd_req)
	{
		cmd_req.set_cmd_type(CMD_RESPOND);
		Command *cmd;

		cmd = cmd_req.add_commands();
		cmd->set_respond_id(network::RESPOND_BULLET);
		cmd->add_args(dstID);
		cmd->add_args(srcID);
		cmd->add_args(hurtSum);
		cmd->add_args(nextID);
	}
    static void useCardNotice(int cardID, int dstID, int srcID, CardMsg& use_card, bool realCard = true)
	{
		use_card.set_type(CM_USE);
		use_card.set_dst_id(dstID);
		use_card.set_src_id(srcID);
		use_card.add_card_ids(cardID);
		use_card.set_is_real(realCard);
	}
	static void showCardNotice(int ID, int howMany, int cardID, CardMsg& show_card)
	{
		show_card.set_type(CM_SHOW);
		show_card.set_src_id(ID);
		show_card.add_card_ids(cardID);
	}
	static void showCardNotice(int ID, int howMany, vector<int> cards, CardMsg& show_card)
	{
		show_card.set_type(CM_SHOW);
		show_card.set_src_id(ID);
		for (int i = 0; i < howMany; i++){
			show_card.add_card_ids(cards[i]);
		}
	}
	static void askForAction(int playerID, int actionTypeAllowed, bool canGiveUp, CommandRequest& cmd_req)
	{
		cmd_req.set_cmd_type(CMD_ACTION);
		Command *cmd;

		if (canGiveUp) {
			cmd = cmd_req.add_commands();
			cmd->set_respond_id(ACTION_NONE);
			cmd->set_src_id(playerID);
		}

		cmd = cmd_req.add_commands();
		cmd->set_respond_id(actionTypeAllowed);
		cmd->set_src_id(playerID);
	}
    static void askForAdditionalAction(int playerID, list<ACTION_QUOTA> quota, CommandRequest& cmd_req)
    {
        cmd_req.set_cmd_type(CMD_RESPOND);
		Command *cmd;

        list<ACTION_QUOTA>::iterator it;
        cmd = cmd_req.add_commands();
		cmd->set_respond_id(network::RESPOND_ADDITIONAL_ACTION);
		cmd->set_src_id(playerID);
        for(it = quota.begin(); it != quota.end(); it++){
            cmd->add_args(it->cause);
        }
		
    }
    static void askForCross(int playerID,int hurtPoint,int type, int crossAvailable, CommandRequest& cmd_req)
	{
		cmd_req.set_cmd_type(CMD_RESPOND);
		Command *cmd;

		cmd = cmd_req.add_commands();
		cmd->set_respond_id(network::RESPOND_HEAL);
		cmd->add_args(playerID);
		cmd->add_args(hurtPoint);
		cmd->add_args(type);
		cmd->add_args(crossAvailable);
	}
	//只填基本信息，若需其他arg，请调用完后再自填
    static void askForSkill(int playerID, int skillID, CommandRequest& cmd_req)
	{
		cmd_req.set_cmd_type(CMD_RESPOND);
		Command *cmd;

		cmd = cmd_req.add_commands();
		cmd->set_respond_id(skillID);
		cmd->set_src_id(playerID);
	}
	static void skillNotice(int srcID, int dstID, int skillID, SkillMsg& skill_msg)
	{
		list<int> dstIDs;
		dstIDs.push_back(dstID);
		skillNotice(srcID, dstIDs, skillID, skill_msg);
	}
	static void skillNotice(int srcID, list<int> dstIDs, int skillID, SkillMsg& skill_msg)
	{
		skill_msg.set_src_id(srcID);
		skill_msg.set_skill_id(skillID);
		list<int>::iterator it;
		for (it = dstIDs.begin(); it != dstIDs.end(); ++it)
		{
			skill_msg.add_dst_ids(*it);
		}
	}
    static void roleNotice(int playerID,int roleID, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info;
		for (int i = 0; i < game_info.player_infos_size(); ++i)
		{
			player_info = (SinglePlayerInfo*)&(game_info.player_infos().Get(i));
			if (player_info->id() == playerID)
			{
				player_info->set_role_id(roleID);
				break;
			}
		}
	}
    static string unactionalNotice(int playerID){return combMessage(TOQSTR(UNACTIONALNOTICE),TOQSTR(playerID));}
    static string notice(string content){return combMessage(TOQSTR(NOTICE),content);}
    static string askForDiscardMagic(int ID){return combMessage("850",TOQSTR(ID));}
    static string askToGiveCard(int ID,int n){return combMessage("750",TOQSTR(ID),TOQSTR(n));}
    static string askForChongYing(int ID,int color){return combMessage("2950",TOQSTR(ID),TOQSTR(color));}
    static void handcardMaxNotice(int ID,int howMany, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);
		player_info->set_max_hand(howMany);
	}
    static void tapNotice(int ID, bool flag, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);
		player_info->set_is_knelt(flag);
	}
    static string specialNotice(int ID,int type,int flag){return combMessage("43",TOQSTR(ID),TOQSTR(type),TOQSTR(flag));}
    static void tokenNotice(int ID,int tokenID,int howMany, GameInfo& game_info)
	{
		SinglePlayerInfo* player_info = game_info.add_player_infos();
		player_info->set_id(ID);
		if (tokenID == 0)
			player_info->set_yellow_token(howMany);
		else
			player_info->set_blue_token(howMany);
	}
	static string askForRolePick(int howMany,int *roles);
    static string coverCardNotice(int playerID,int howMany,vector < int > cards,bool remove,bool show);
    static string askForSkillNumber(int playerID,int skillNum){return combMessage(TOQSTR(skillNum));}
    static string optionalRoleNotice(int num, int *roles);
    static string askForBan(int ID);
    static string banNotice(int ID, int role);
    static string askForPick(int ID);
    static string pickNotice(int ID, int role);
    static string nicknameNotice(int id,string name){return combMessage("58",TOQSTR(id),name);}

};

CardEntity* getCardByID(int id);