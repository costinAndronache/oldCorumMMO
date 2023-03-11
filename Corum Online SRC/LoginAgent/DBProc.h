#ifndef __DBPROC_H__
#define __DBPROC_H__

#pragma once

#include "GlobalDefine.h"
#include "../CommonServer/CommonHeader.h"
#include "User.h"

#pragma pack(push,1)

/*
<-----     Login Procedure Return Value For Corum     ------>
SELECT  0, 0, @propid, 	1, @eday , 0, 0 ,@id_regdate    --�α��� ����	
SELECT  1, 0, 0,		0, @eeday, 0, 0 ,@id_regdate 	--SQL ��� ����,�α��ν���
SELECT  2, 0, 0, 		0, @eeday, 0, 0 ,@id_regdate 	--���� ���̵� �Դϴ�.
SELECT  3, 0, 0, 		0, @eeday, 0, 0 ,@id_regdate 	--��й�ȣ�� Ʋ���ϴ�.
SELECT  4, 0, @propid, 	0, @eeday, 0, 0 ,@id_regdate 	--�̹� ���ӿ� ���� ���Դϴ�. 
SELECT  5, 0, @propid, 	0, @eeday, 0, 0 ,@id_regdate 	--���� �� ����
SELECT  6, 0, @propid,  0, @eeday, 0, 0 ,@id_regdate 	--14���̸� �ӽð����ڷ� �θ� ���� ó�� �� ���Ӱ����մϴ�.
*/

/*
<-----     Login Procedure Return Value For NetMarble     ------>
select 0,0,@idx,1,getdate() ,0,0, @id_regdate	-- ����
select 3,0,0,	0,getdate() ,0,0, @id_regdate	-- ��й�ȣ Ʋ�� 	
select 4,0,@idx,0,getdate() ,0,0, @id_regdate	-- �̹� �α��εǾ�����
select 5,0,@idx,0,getdate() ,0,0, @id_regdate	-- �ҷ�������
select 6,0,@idx,0,getdate() ,0,0, @id_regdate	-- SQL ����.
*/

struct LOGIN_RESULT
{
	DWORD dwResult;				//�α���/(0:������ ���, 1:�α���ok, 2:���� ���̵�, 3:Ʋ�� ���, 4:�α������� ���̵�, 5:��������, 6:�׽��� �ƴϴ�, 7:�̹� PC�� ������ 1�� �����)
	DWORD dwPrevUserServerSet;	//�ߺ��α��ν� ���� ������ �α��� �Ǿ��ִ� ������ ��ȣ 
	DWORD dwPropID;				//propid
	DWORD dwRestrictEx;			//�������(0:�������ÿ�, 10:�����ǹ���, 9:1�ð�����, 2:���� ��¥�����Ƽ� ���, 1:pc�� �������, 3:pc�� �������, 4: ���� ���� ��� 
	DBTIMESTAMP eday;			//eday	
	DWORD dwRemainTime;			//������ �϶� �����ð� 
	DWORD dwCoupon;				//ó�� ȸ�������� ���� 14�� ü���� ��밡�� ���� (1:��밡��, 2:���������� ���Ұ�, 3:���������� ���Ұ�)
	DBTIMESTAMP id_regdate ;   //���� ������ ( id_regdate ) ��� 050323
};

struct LOGIN_RESULT_WITH_PROPID
{
	DWORD dwResult;				//�α���/(0:������ ���, 1:�α���ok, 2:���� ���̵�, 3:Ʋ�� ���, 4:�α������� ���̵�, 5:��������, 6:�׽��� �ƴϴ�, 7:�̹� PC�� ������ 1�� �����)
	DWORD dwPrevUserServerSet;	//�ߺ��α��ν� ���� ������ �α��� �Ǿ��ִ� ������ ��ȣ 
	char  szPropID[15];			//propid
	DWORD dwRestrictEx;			//�������(1:ip eday, 2:������ eday, 3:ip time, 4:������ time, 5:������)
	DBTIMESTAMP eday;			//eday	
	DWORD dwRemainTime;			//������ �϶� �����ð� 
	DWORD dwCoupon;				//ó�� ȸ�������� ���� 14�� ü���� ��밡�� ���� (1:��밡��, 2:���������� ���Ұ�, 3:���������� ���Ұ�)
	DBTIMESTAMP id_regdate ;   //���� ������ ( id_regdate ) ��� 050323
};

struct JUMIN_CHECK_RESULT
{
	DWORD dwResult;
	char  szCharacterName[20];
};

struct BLOCK_TITLE
{
	DWORD dwType;
	char  szReason[0xff];
};

struct INIT_BINARY_VALUE
{
	CItem			m_pEquip[ MAX_EQUIP ];
	CItem			m_pInv_Large[ MAX_INV_LARGE ];
	CItem			m_pInv_Small[ MAX_INV_SMALL ];
	CItem			m_pBelt[ MAX_BELT ];
	BYTE			m_pwSkillLevel[ MAX_SKILL ];
	DWORD			dwMoney;
};

struct CREATE_CHARACTER_RESULT
{
	DWORD dwResult;
	DWORD dwChar_Index;
	DWORD dwCharCount; //��� 050323 
};

struct DELETE_CHARACTER_RESULT
{
	DWORD	dwResult;
	char	szDeletedCharacterName[20];
};

struct BILLING_CHECK_RESULT
{
	DWORD dwPropID;
	DWORD nBillingType;
	DBTIMESTAMP eday;
	DWORD	dwRemainTime;
	DWORD	dwContinue;		//0�̸� �� ������ �����ؾ� �ȴ�.. ©���..

};

#pragma pack(pop)

struct DBRECEIVEDATA;

void QueryTypeLogin(DBRECEIVEDATA* pResult);
void QueryTypeLoginFree(DBRECEIVEDATA* pResult);
void QueryTypeChrSelectInfo(DBRECEIVEDATA* pResult);
void QueryTypeCheckMonitoring(DBRECEIVEDATA* pResult);
void QueryTypeIsAliveUser(DBRECEIVEDATA* pResult);
void QueryTypeBillingCheck(DBRECEIVEDATA* pResult);
void QueryTypeBankItemCheck(DBRECEIVEDATA* pResult);
void QueryTypeCreateNewChar(DBRECEIVEDATA* pResult);
void QueryTypeDeleteCharacter(DBRECEIVEDATA* pResult);
void QueryTypeJuminCheck(DBRECEIVEDATA* pResult);
void QueryTypeCreateCharInitBinary(DBRECEIVEDATA* pResult);
void QueryTypeGMCheck(DBRECEIVEDATA* pResult);
void QueryTypeBlockTitle(DBRECEIVEDATA* pResult);
void QueryTypeWarningTitle(DBRECEIVEDATA* pResult);


#endif // __DBPROC_H__