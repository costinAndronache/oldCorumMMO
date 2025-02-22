//#include	"../../4DyuchiGXExecutive/GXObject.h"
#include	"../myself97common.h"
#include	"../CheckClock.h"
#include	"SphereCollisionTest.h"
#include	"../../4DyuchiGXGFunc/global.h"
#include	"../../4DyuchiGXExecutive/GXEventTriggerObject.h"

// 충돌 처리용 전역들.
struct _Collision
{
	// static object용 삼각형 인덱스 버퍼.
#define		cMaxTriCount			(400)
#define		cMaxSObjCount			(100)
	CGXStaticMapObject*				pSObj[cMaxSObjCount];
	DWORD							dwSObj;
	DWORD							pTempTriIndex[cMaxTriCount];
	VECTOR3							pTri[cMaxTriCount*3];
	BYTE							pbTriChecked[cMaxTriCount];		// 체크 버퍼.
	DWORD							dwTriCount;

	// dynamic object용 인덱스 버퍼.
#define		cMaxObjCount			(600)
	CoGXObject*						pDFound[cMaxObjCount];
	BYTE							pbObjCheck[cMaxObjCount];
	DWORD							dwDObj;

	// 이벤트박스.
#define		cMaxEventBoxCount		(50)
	CoGXEventTrigger*				pEventBox[cMaxEventBoxCount];
	DWORD							dwEventBoxCount;
} gColl;

//struct		DYNAMIC_COLLISION_IN
struct		COLLISION_IN
{
	VECTOR3		From;
	VECTOR3		To;
	float		fRadius;
};

struct		COLLISION_OUT
{
	VECTOR3		MeetPivot;		// 만났을때의 피봇.
	float		fMeetTime;		// 만났을때의 from-to간 시점.

	VECTOR3		WhereMeet;		// 만난 접점.
	PLANE		MeetPlane;		// 만난 접점의 평면.
};

//	실제로 충돌처리 하는 함수.
BOOL	TestMovingSphereMeetSphere( COLLISION_OUT* pOut,  COLLISION_IN* pIn, BOUNDING_SPHERE* pTarget);

BOOL	TestMovingSphereMeetTri( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri);
BOOL	TestMovingSphereMeetTriEdge( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri);
BOOL	TestMovingSphereMeetLine( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pL1, VECTOR3* pL2);
BOOL	TestMovingSphereMeetPLANE( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri);
BOOL	TestMovingSphereMeetVertex( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pV);


//	푸쉬~.
//	pOutCandidate는 밀린 후의 새로운 to 후보, pIn은 상황 입력용.
void	PushSphere(VECTOR3* pOutCandidate, COLLISION_OUT* pIn, VECTOR3* pTo, float fRadius);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	SphereCollisionTest(CoExecutive* pExecutive, CoGXObject* pThis, COLLISION_TEST_RESULT* pCollTestResult, VECTOR3* pTo, DWORD dwBufferIndex)
{
	pCollTestResult->LastVelocity.x		=	0.0f;
	pCollTestResult->LastVelocity.y		=	0.0f;
	pCollTestResult->LastVelocity.z		=	0.0f;

	pCollTestResult->bLand				=	FALSE;

	pCollTestResult->Candidate		=	*pTo;

	DWORD		dwClock;
	BOOL		bResult;

	MClipper*			pClipper	=	pExecutive->GetGXMap()->GetClipper();
	GXSCHEDULE_PROC_MSG_COLLISION_ARG	CallBack;			// 충돌이 일어났을 때 pThis에 날려줄 콜백 정보.

	COLLISION_IN	in;
	COLLISION_OUT	out;

	GXProcedureHandler* pProc = pThis->GetProcedureHandler();			// pThis의 프로시져.
	
	VECTOR3		ObjPivot;		// 오브젝트의 중점. 충돌처리 하는동안 계속 변한다.
	VECTOR3		ObjTo;			// 오브젝트의 목적지. 역시 충돌처리 하는동안 계속 변한다.
	VECTOR3		ObjVelocity;	// 오브젝트의 속도벡터. 
	float		fObjVelocity;	// 오브젝트의 속도. 스칼라.

	BOUNDING_VOLUME*	pbv	=	pThis->GetBoundingVolume();
	in.fRadius		=	pbv->fRadius;


	// 피봇 처리.
	pThis->GetPosition(&ObjPivot);
	VECTOR3_ADDEQU_VECTOR3(&ObjPivot, &(pbv->vPivot));
	ObjTo		=	*pTo;
	VECTOR3_ADDEQU_VECTOR3(&ObjTo, &(pbv->vPivot));
	VECTOR3_SUB_VECTOR3( &ObjVelocity, &ObjTo, &ObjPivot);
	fObjVelocity	=	VECTOR3Length( &ObjVelocity);

	// 근처 오브젝트 찾는다.
	BOUNDING_SPHERE		bs;
	bs.v3Point		=	ObjTo;
	bs.fRs			=	fObjVelocity + pbv->fRadius + 10.0f;		// 10.0f는 마진... -_-;

	// 실제 트리 타고,
	dwClock	=	GetLowClock();
	pClipper->FindNearObject(bs);

	dwClock		=	GetLowClock() - dwClock;

	// 다이나믹 오브젝트가 저장될 버퍼.
	CoGXObject**		ppFoundDObjTmp;
	gColl.dwDObj	=	pClipper->GetTargetBuffer( (CGXMapObject***)&ppFoundDObjTmp, GX_MAP_OBJECT_TYPE_OBJECT);
	// 버퍼보다 더 많이 들어온 경우 체크.
	if( gColl.dwDObj > cMaxObjCount)
		_asm int 3;
	// 이쪽의 버퍼로 카피한다.
	if( gColl.dwDObj)
		memcpy( gColl.pDFound, ppFoundDObjTmp, sizeof(CGXMapObject*)*gColl.dwDObj);

	// 스테이틱 맵 오브젝트.
	if( (pbv->dwCollisionTargetFlag & COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT) == 0)
//	if( !(pbv->dwCollisionTargetFlag & COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT))
		goto lbPassStaticTree;
	CGXStaticMapObject**	ppFoundSObj;
	gColl.dwSObj	=	pClipper->GetTargetBuffer( (CGXMapObject***)&ppFoundSObj, GX_MAP_OBJECT_TYPE_STRUCT);
	// 버퍼보다 더 많이 들어온 경우.
	if( gColl.dwSObj > cMaxSObjCount)
		_asm int 3;
	// 버퍼 카피.
	if( gColl.dwSObj)
		memcpy( gColl.pSObj, ppFoundSObj, sizeof(CGXStaticMapObject*)*gColl.dwSObj);

	// 스테이틱 오브젝트를 돌면서 삼각형들을 찾아낸다.

	gColl.dwTriCount	=	0;					// 삼각형 갯수.

	DWORD	dwSObjIndex;
	for( dwSObjIndex = 0; dwSObjIndex < gColl.dwSObj; dwSObjIndex++)
	{
		// 스테이틱 오브젝트의 트리를 얻어서,
		MStaticObjectTree*	pTree	=	gColl.pSObj[dwSObjIndex]->GetTree();
		// 지금 오브젝트가 있는 근처의 삼각형을 버퍼에 카피.
		DWORD	dwTempTriCount	=	pTree->FindTriWithSphere( gColl.pTempTriIndex, cMaxTriCount, &bs, FALSE);	// 이 옵션은 바꿔볼 필요가 있다.
		DWORD	dwTempIndex;
		for( dwTempIndex = 0; dwTempIndex < dwTempTriCount; dwTempIndex++)
		{
			memcpy( &(gColl.pTri[gColl.dwTriCount*3]), &(pTree->GetTriBuffer()[gColl.pTempTriIndex[dwTempIndex]*3]), sizeof(VECTOR3)*3);
			gColl.dwTriCount++;
		}
	}
lbPassStaticTree:

	// 스테이틱 오브젝트에서 삼각형을 다 뽑아냈다.
	
	gColl.dwEventBoxCount =	pClipper->GetTargetBuffer( (CGXMapObject***)&ppFoundDObjTmp, GX_MAP_OBJECT_TYPE_TRIGGER);
	if( gColl.dwEventBoxCount > cMaxEventBoxCount)
		_asm int 3;
	if( gColl.dwEventBoxCount)
		memcpy( gColl.pEventBox, ppFoundDObjTmp, sizeof(CGXMapObject*)*gColl.dwEventBoxCount);


	DWORD				dwLastMeetObjType	=	0xffffffff;
	DWORD				dwLastMeetObjNum	=	0xffffffff;
	CGXMapObject*		pLastMeetObj		=	0;
	COLLISION_OUT		lastMeetOut;
	memset( &lastMeetOut, 0, sizeof(COLLISION_OUT));
	BOOL				bFound;

	// 체크 버퍼 클리어.
	memset( gColl.pbObjCheck, 0, sizeof(BYTE)*gColl.dwDObj);
	memset( gColl.pbTriChecked, 0, sizeof(BYTE)*cMaxTriCount);
	

	DWORD		dwTempLoopCount	=	0;
	while(1)	// 루프를 돌면서,
	{
		dwTempLoopCount++;
		bFound						=	FALSE;
		lastMeetOut.fMeetTime		=	1.0f;
		// 대 다이나믹 오브젝트.
		if( pbv->dwCollisionTargetFlag&COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT )
		{
			for( DWORD i = 0; i < gColl.dwDObj; i++)
			{
				if( gColl.pDFound[i] == pThis)		// 자기 자신이면 패스.
					continue;
				
				BOUNDING_VOLUME* pTargetBV	=	gColl.pDFound[i]->GetBoundingVolume();		// 타겟.
				VECTOR3		TargetPivot;
				gColl.pDFound[i]->GetPosition( &TargetPivot);
				TargetPivot	=	TargetPivot + pTargetBV->vPivot;
				

				// 오브젝트 타입을 체크.
				if( pTargetBV->dwType == BOUNDING_VOLUME_TYPE_NONE)
				{
					continue;				
				}

				// 바운딩 스피어라고 가정한다.
				BOUNDING_CAPSULE		bc;
				bc.v3From		=	ObjPivot;
				bc.v3To			=	ObjTo;
				bc.fRadius		=	pbv->fRadius;

				// 충돌 영역내에 있는가.? 검사해서 패스.
				bs.fRs			=	pTargetBV->fRadius;
				bs.v3Point		=	TargetPivot;
				int	iResult		=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, bs);

				if( iResult < 0)
					continue;

				// 실제로 충돌을 하는가 검사한다.
				in.fRadius	=	pbv->fRadius;
				in.From		=	ObjPivot;
				in.To		=	ObjTo;
//				in.Target	=	bs;
				bResult		=	TestMovingSphereMeetSphere( &out, &in, &bs);
				if( bResult )
				{
					// 시점값 t 체크. 가장 가까운 놈인가.?
					if( out.fMeetTime < lastMeetOut.fMeetTime)
					{
						pLastMeetObj	=	gColl.pDFound[i];
						lastMeetOut		=	out;
						bFound			=	TRUE;
						dwLastMeetObjNum	=	i;
						dwLastMeetObjType	=	COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT;
					}
				}
				
			}
		} // if
		

		// 대 스테이틱 오브젝트의 삼각형.
		if( pbv->dwCollisionTargetFlag&COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT)
		{
			// 삼각형 리스트를 돌아가면서,
			DWORD	dwTriIndex;
			for( dwTriIndex = 0; dwTriIndex < gColl.dwTriCount; dwTriIndex++)
			{
				// 충돌 테스트.
				in.fRadius	=	pbv->fRadius;
				in.From		=	ObjPivot;
				in.To		=	ObjTo;
				BOOL bMeetTri	=	TestMovingSphereMeetTri( &out, &in, &(gColl.pTri[dwTriIndex*3]));
				if( bMeetTri)
				{
					if( out.fMeetTime < lastMeetOut.fMeetTime) // 가장 최근의 t와 비교, 더 가까우면 바꾼다.
					{
						if( out.fMeetTime < -0.01f)			// float의 약간의 오차때문에 유예를 둔다.
						{
							VECTOR3		Gab;
							VECTOR3_SUB_VECTOR3( &Gab, &(in.From), &(out.MeetPivot));
							float		fGab	=	VECTOR3Length( &Gab);
							if( fGab > 0.1f)
							{
								_asm int 3;		// 그런데 그게 황당할 정도로 빗나가면 좀 곤란하다.
							}
						}
						lastMeetOut	=	out;
						bFound		=	TRUE;
						dwLastMeetObjType			=	COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT;
//						dwLastMeetComponentType		=	dwType;
//						gColl.pbTriChecked[dwTriIndex]		=	TRUE;
						dwLastMeetObjNum	=	dwTriIndex;
					}
				}
				_asm nop;
			}
		}

		if( bFound )		// 하나라도 만난게 있다.
		{
			// 이미 만났던 놈인가.?
			if( dwLastMeetObjType	==	COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT)
			{
				if( gColl.pbObjCheck[dwLastMeetObjNum] == TRUE)
				{
					_asm nop;
					// 충돌 끝내고 딱 여기까지 민 다음 리턴.
//					*pCandidate		=	lastMeetOut.MeetPivot;
					pCollTestResult->Candidate	=	lastMeetOut.MeetPivot;
					goto	lbReturn;
				}
				// 그게 아니라면 충돌 처리.
				gColl.pbObjCheck[dwLastMeetObjNum]	=	TRUE;				// 체크하고,
			}



			// 만약 오브젝트 타입이 건물일 경우, 만났던 삼각형과 또 만났나.?
			if(dwLastMeetObjType == COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT)
			{
				if( gColl.pbTriChecked[dwLastMeetObjNum] == TRUE)
				{
					_asm nop;
//					*pCandidate		=	lastMeetOut.MeetPivot;
//					goto	lbReturn;
				}
				_asm nop;
				gColl.pbTriChecked[dwLastMeetObjNum]		=	TRUE;
			}

			// 프로시저 콜. 어떻게 행동할지 결정한다.
			CallBack.dwCollisionType	=	GX_MAP_OBJECT_TYPE_OBJECT;
			CallBack.gxHandle			=	gColl.pDFound[dwLastMeetObjNum];
			CallBack.vMeetPos			=	lastMeetOut.MeetPivot;
			CallBack.vWhereMeet			=	lastMeetOut.WhereMeet;
//			CallBack.vToCandidate		=	ObjTo;
			CallBack.MeetPlane.v3Up		=	lastMeetOut.MeetPivot - lastMeetOut.WhereMeet;
			Normalize( &CallBack.MeetPlane.v3Up, &CallBack.MeetPlane.v3Up);
			CallBack.MeetPlane.D		=	-1.0f * ( CallBack.MeetPlane.v3Up * lastMeetOut.WhereMeet );
			DWORD dwProcedureReturn	=	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE;		// 기본값.
			if( pProc)
				dwProcedureReturn	=	pProc->GXSchedulePROC( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_COLLISION, DWORD( &CallBack), 0, pThis->GetData());
			switch( dwProcedureReturn)		// 프로시저 리턴값에 따른 행동 양식.
			{
				case	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_STOP:
//					*pCandidate		=	lastMeetOut.MeetPivot;
					pCollTestResult->Candidate		=	lastMeetOut.MeetPivot;
					goto	lbReturn;
				case	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE:
					// 찾아진 평면으로 푸쉬~
					VECTOR3		NewTo;
					PushSphere( &NewTo, &lastMeetOut, &ObjTo, pbv->fRadius);
					// 새로운 From과 To.
					ObjPivot	=	lastMeetOut.MeetPivot;
					ObjTo		=	NewTo;

					break;
//				case	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_REFLECT:
//					break;
				default:
					_asm int 3;
			}

		}
		else	// 만난게 하나도 없다면 이제 그만~
		{
//			*pCandidate		=	ObjTo;
			pCollTestResult->Candidate		=	ObjTo;
			goto	lbReturn;
		}

		// 충돌 끝낼 조건인가.?
		ObjVelocity		=	ObjTo	-	ObjPivot;
		fObjVelocity	=	VECTOR3Length( &ObjVelocity);
		if( fObjVelocity < 1.0f)
		{
//			*pCandidate	=	ObjPivot;
			pCollTestResult->Candidate		=	ObjPivot;
			goto	lbReturn;
		}

		// 속도 벡터를 약간 줄인다.
		Normalize( &ObjVelocity, &ObjVelocity);
		fObjVelocity	*=	0.99f;
		ObjVelocity	=	ObjVelocity	*	fObjVelocity;
		ObjTo	=	ObjPivot	+	ObjVelocity;

		if( dwTempLoopCount > 100)
		{
			_asm int 3;				// 너무 루프를 많이 돌았다.
		}
	} //while

lbReturn:
	// 마지막으로 최종 위치와 이벤트박스가 만나는가 검사한다.
	BOUNDING_SPHERE	ThisSphere;
//	ThisSphere.v3Point	=	*pCandidate;
	ThisSphere.v3Point		=	pCollTestResult->Candidate;
	ThisSphere.fRs		=	pbv->fRadius;
	DWORD	dwEventIndex;
	for( dwEventIndex = 0; dwEventIndex < gColl.dwEventBoxCount; dwEventIndex++)
	{
		BOUNDING_BOX*	pBox	=	&(gColl.pEventBox[dwEventIndex]->GetCollisionMeshObjectDesc()->boundingBox);
		BOOL	bMeetBox	=	IsCollisionSphereAndBox( &ThisSphere,  pBox);

		if( bMeetBox)		// 만났을 경우,
		{
			DWORD	dwTriggerProcReturn	=	0;
			GXProcedureHandler* pTriggerProc	=	gColl.pEventBox[dwEventIndex]->GetProc();
			if( pTriggerProc)
				pTriggerProc->GXSchedulePROC( pExecutive, (GXOBJECT_HANDLE)gColl.pEventBox[dwEventIndex], GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, (DWORD)pThis, 0, gColl.pEventBox[dwEventIndex]->GetData());


//			DWORD	dwEventTriggerReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, DWORD( &TriggerArg), 0, pThis->GetData());

			if( pProc)
			{
				GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER_ARG		TriggerArg;
				TriggerArg.dwEventTriggerID		=	gColl.pEventBox[dwEventIndex]->GetID();
//				dwTriggerProcReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, (DWORD)gColl.pEventBox[dwEventIndex], 0, pThis->GetData());
				dwTriggerProcReturn	=	pProc->GXSchedulePROC( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, DWORD( &TriggerArg), 0, pThis->GetData());
			}
		}
	}


	VECTOR3_SUBEQU_VECTOR3( &(pCollTestResult->Candidate), &(pbv->vPivot));

	if( dwTempLoopCount > 5)
	{
		_asm nop;
	}

	return	;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	중점		:	X = F + (T-F)*t;
	반지름		:	r1;
	
	타겟중점	:	P;
	타겟반지름	:	r2;
	
	|P-X| = r1+r2;
	식 전개하면,
	t^2*(T-F)(T-F) + 2*t*(F(T-F)-P(T-F)) + PP - 2*PF + FF - (r1+r2)*(r1+r2) = 0;
	    ----------       ---------------   --------------------------------
			= a                 = b                        = c
*/
BOOL TestMovingSphereMeetSphere( COLLISION_OUT* pOut,  COLLISION_IN* pIn, BOUNDING_SPHERE* pTarget)
{
	VECTOR3		vTemp;

//	float	r	=	pIn->fRadius	+	pIn->Target.fRs;
	float	r	=	pIn->fRadius	+	pTarget->fRs;

	VECTOR3		Velocity;
	VECTOR3_SUB_VECTOR3( &Velocity, &pIn->To, &pIn->From);

	float	a	=	DotProduct( &Velocity, &Velocity);
	float	b	=	DotProduct( &(pIn->From), &Velocity)	-	DotProduct(&(pTarget->v3Point), &Velocity);
//	float	c	=	DotProduct( &(pTarget->v3Point),&(pIn->Target.v3Point)) - 2 * DotProduct( &(pIn->Target.v3Point), &(pIn->From)) + DotProduct( &(pIn->From), &(pIn->From)) - pIn->fRadius * pIn->fRadius;
	float	c	=	DotProduct( &(pTarget->v3Point),&(pTarget->v3Point)) - 2 * DotProduct( &(pTarget->v3Point), &(pIn->From)) + DotProduct( &(pIn->From), &(pIn->From)) - r*r;

	float	fDet	=	b*b-a*c;		// 판별식.
	if( fDet < -0.00001f)
	{
		// 만나지 않는다.
		return	FALSE;
	}
	else	if( fDet > 0.00001f)	// 두점에서 만난다. 아무래도 대부분 여기 걸릴듯.
	{
		// 근의 공식.	t = (-b - root(b*b-a*c))/a;
		//				t = (-b + root(b*b-a*c))/a; 는 이미 지나간 상태에서 만난다는 것이므로 의미가 없다. 
		pOut->fMeetTime		=	( -1.0f*b - (float)sqrt(b*b - a*c) ) / a;
	}
	else		// -0.0001f <= fDet <= 0.0001f 한점에서 만난다.
	{
		// 근의 공식.	t = (-b) / a;
		pOut->fMeetTime		=	(-1.0f*b) / a;
	}


	// 이제 t를 검사해 이 충돌이 유효한지를 알아낸다.
	if( pOut->fMeetTime > 1.0f)
	{
		// 너무 멀리서 만났다. 
		return	FALSE;
	}
	
	if( pOut->fMeetTime < 0.0f)	//	'-'가 나왔을 경우.
	{
		// 부동 소수점의 오차때문에, 만난 시점이 '-'일 경우는 이미 어떤 물체에 닿아있을 수가 있으므로 체크해서,
		// 만나고 있다면 time값을 '0'으로 맞추고 '0'의 시점에서 움직일 방향을 잡아낸다.

		//	어버버버버버버~



		VECTOR3		From2Target;			// 타겟의 피봇 - 스피어From;
		VECTOR3_SUB_VECTOR3( &From2Target, &(pTarget->v3Point), &(pIn->From));
		Normalize( &From2Target, &From2Target);

		VECTOR3		VelocityNormal;
		Normalize( &VelocityNormal, &Velocity);			// 스피어의 속도.
	
		fDet	=	DotProduct( &From2Target, &VelocityNormal);		// 만나는 방향과 스피어의 속도 내적, 방향 테스트.
		if( fDet <= 0.0f)		// 진행 방향이 만나는 면과 평행하거나 멀어지는 방향일 경우.
		{
			return	FALSE;
		}

		// 약간 겹치는걸로 판별.
		if( pOut->fMeetTime < 0.01f)
		{
			_asm nop;			// 도대체 어느정도나 들어갈 것인가.?
		}

	}

	//	만났다고 판별되는 경우 pOut을 채우고 리턴. 역시 pOut->MeetPlane은 함수 외부에서 채운다.

//	pOut->MeetPivot	=	pIn->From	+	(pIn->To - pIn->From)*pOut->fMeetTime
	VECTOR3_SUB_VECTOR3( &vTemp, &(pIn->To), &(pIn->From));
	VECTOR3_MULEQU_FLOAT( &vTemp, pOut->fMeetTime);
	VECTOR3_ADD_VECTOR3( &(pOut->MeetPivot), &(pIn->From), &vTemp);
//	pOut->WhereMeet		=	pOut->MeetPivot + (pIn->Target.v3Point - pOut->MeetPivot) * (pIn->fRadius) / (pIn->fRadius + pIn->Target.fRs);
	VECTOR3		Candidate2Target;
	VECTOR3_SUB_VECTOR3( &Candidate2Target, &(pTarget->v3Point), &(pOut->MeetPivot));
	VECTOR3_MUL_FLOAT( &vTemp, &Candidate2Target, (pIn->fRadius) / (pIn->fRadius + pTarget->fRs));
	VECTOR3_ADD_VECTOR3( &(pOut->WhereMeet), &(pOut->MeetPivot), &vTemp);
	// X*N+d = 0;
//	Normalize( &(pOut->MeetPlane.v3Up), &Candidate2Target);
//	VECTOR3_MULEQU_FLOAT( &(pOut->MeetPlane.v3Up), -1.0f);
//	pOut->MeetPlane.D	=	-1 * DotProduct( &(pOut->MeetPlane.v3Up), &(pOut->WhereMeet));

	return	TRUE;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	푸쉬~.
//	pOutCandidate는 밀린 후의 새로운 to 후보, pIn은 상황 입력용.
void	PushSphere(VECTOR3* pOutCandidate, COLLISION_OUT* pIn, VECTOR3* pTo, float fRadius)
{
	// 움직이지 않은 상태의 스피어 중점과 평면과의 거리를 계산한다. 
	float	a	=	pIn->MeetPivot	*	pIn->MeetPlane.v3Up	+	pIn->MeetPlane.D;

	if( a < fRadius)	// 만약 평면에 스피어가 이미 파고 들어가있나.?
	{
		//	이런 결과가 나와선 안된다.
		//	충돌 테스트 하는 함수에서 처음부터 t를 '-'를 내놓으면 안된다.
//		_asm int 3;
//		a		=	fRadius;			// 어느 a가 옳은가?
	}

	//	목표 지점으로 움직인 후의 평면과의 거리, 대충 '+', '0', '-' 다 나올 수 있다.
	float	b	=	*pTo	*	pIn->MeetPlane.v3Up	+	pIn->MeetPlane.D;

	*pOutCandidate	=	*pTo	+	pIn->MeetPlane.v3Up * (a - b);	// 밀어낼 포인트를 결정 하고,

	// 리턴.
	return	;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetTri( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri)
{
	BOOL	bResult;
	BOOL	bFound	=	FALSE;		// 삼각형과 만나긴 만났는가.?
	COLLISION_OUT		out;

	DWORD	dwLastMeetComponent	=	0xffffffff;

	// 평면과 만나는 점.?
	bResult	=	TestMovingSphereMeetPLANE( &out, pIn, pTri);		// 리턴값 중 out.MeetPlane은 무효.
	if( bResult == TRUE)
	{
		dwLastMeetComponent	=	0;
		bFound	=	TRUE;
		*pOut	=	out;
		CheckVectorArray( &(out.MeetPivot), 1);
	}


	// 엣지와 만나는 점.?
	bResult	=	TestMovingSphereMeetTriEdge( &out, pIn, pTri);	// 리턴값 중 out.MeetPlane은 무효.
	if( bResult == TRUE)
	{
		if( (bFound == TRUE && pOut->fMeetTime > out.fMeetTime) || bFound == FALSE)
		{
			dwLastMeetComponent	=	1;
			bFound	=	TRUE;
			*pOut	=	out;
			CheckVectorArray( &(out.MeetPivot), 1);
		}
	}


	// 모서리와 만나는 점?
	int	i;
	for( i = 0; i < 3; i++)
	{
		bResult	=	TestMovingSphereMeetVertex( &out, pIn, &(pTri[i]));		// 리턴값 중 out.MeetPlane은 무효.
		if( bResult == TRUE)
		{
			if( (bFound == TRUE && pOut->fMeetTime > out.fMeetTime) || bFound == FALSE)
			{
				dwLastMeetComponent	=	2;
				bFound	=	TRUE;
				*pOut	=	out;
				CheckVectorArray( &(out.MeetPivot), 1);
			}
		}
	}



/*lbReturn:*/

	// 만난게 있는가.?
	if( bFound == FALSE)
	{
		return	FALSE;
	}

	// 만나는 면을 계산한다.

	CheckVectorArray( &(pOut->MeetPivot), 1);


	pOut->MeetPlane.v3Up	=	pOut->WhereMeet	-	pOut->MeetPivot;
	Normalize( &pOut->MeetPlane.v3Up, &pOut->MeetPlane.v3Up);
	pOut->MeetPlane.D		=	-1.0f * DotProduct( &pOut->MeetPlane.v3Up, &pOut->WhereMeet);


//	if( pOut->MeetPlane.v3Up.x < 600000.0f)
//		_asm int 3;

	return	TRUE;			// 끝.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetTriEdge( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri)
{
	// 삼각형의 각 엣지를 돌아보다.
	float	t	=	1.0f;

	BOOL	bResult;
	BOOL	bFound	=	FALSE;
	COLLISION_OUT	Out;

	bResult	=	TestMovingSphereMeetLine( &Out, pIn, &pTri[0], &pTri[1]);
	if( bResult == TRUE)
	{
		CheckVectorArray( &(Out.MeetPivot), 0);
		CheckVectorArray( &(Out.MeetPlane.v3Up), 0);
		if( Out.fMeetTime < t)
		{
			bFound	=	TRUE;
			t	=	Out.fMeetTime;
			*pOut	=	Out;
			CheckVectorArray( &(pOut->MeetPivot), 1);
		}
	}

	bResult	=	TestMovingSphereMeetLine( &Out, pIn, &pTri[1], &pTri[2]);
	if( bResult == TRUE)
	{
		CheckVectorArray( &(Out.MeetPivot), 0);
		CheckVectorArray( &(Out.MeetPlane.v3Up), 0);
		if( Out.fMeetTime < t)
		{
			bFound	=	TRUE;
			t	=	Out.fMeetTime;
			*pOut	=	Out;
			CheckVectorArray( &(pOut->MeetPivot), 1);
		}
	}

	bResult	=	TestMovingSphereMeetLine( &Out, pIn, &pTri[2], &pTri[0]);
	if( bResult == TRUE)
	{
		CheckVectorArray( &(Out.MeetPivot), 0);
		CheckVectorArray( &(Out.MeetPlane.v3Up), 0);
		if( Out.fMeetTime < t)
		{
			bFound	=	TRUE;
			t	=	Out.fMeetTime;
			*pOut	=	Out;
			CheckVectorArray( &(pOut->MeetPivot), 1);
		}
	}

	if( bFound == FALSE)
	{
		return	FALSE;
	}

			CheckVectorArray( &(pOut->MeetPivot), 1);


	CheckVectorArray( &(pOut->MeetPivot), 0);
	CheckVectorArray( &(pOut->MeetPlane.v3Up), 0);


	return	TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	2003.03.19	t가 '-'일 경우 다시 만나고 있는지 체크하게 고치다. 이 방법밖엔 없나. -_-;
	2003.03.18	t가 '-'일 경우 리턴으로 고치다.
	움직이는 직선이 라인과 만나는가.?

	직선은		M = A + B*s;
	구의 궤적은 P = C + D*t;

	이때
	1. B*(M-P) = 0;
	2. (P-M)*(P-M) = r^2
	두개가 나온다.

	식1에 M, P를 대입하여 s = ~~  식으로 유도한 뒤,
	직선 M = A + B*s에 대입하여 M을 t에 관한 식으로 유도.

	M과 P를 식2에 대입해서, (E+F*t)*(E+F*t) = r^2 으로,

	F*F*t^2 + 2*E*F*t + E*E-r^2 = 0
	 = a        = b       = c

	판별식 det = b^2 - 4*a*c;

	근이 두개일때 t1 = (-b - root(det)) / 2a 하나만 사용 (t2는 의미가 없다.)

    이때 0 < s < 1 이 아니면 근은 의미가 없다.
*/

//분명히 이놈이 문제가 있다.
BOOL	TestMovingSphereMeetLine( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pL1, VECTOR3* pL2)
{
	VECTOR3		A	=	*pL1;		// LineFrom
	VECTOR3		B;									// LineVelocity
	VECTOR3_SUB_VECTOR3( &B, pL2, pL1);

//	VECTOR3_SUBEQU_VECTOR3( &B, &A);
	VECTOR3		M;									// Line위의 한점.
	float		s;									// 그때의 시점.

	VECTOR3		C	=	pIn->From;					// Sphere From.
	VECTOR3		D	=	pIn->To;					// Sphere Velocity.
	VECTOR3_SUBEQU_VECTOR3( &D, &C);
	VECTOR3		P;									// Sphere위의 한점.
	float		t = 0.0f;									// 그때의 시점.

	float		dotBB	=	DotProduct( &B, &B);
	float		fTemp	=	(DotProduct( &B, &C) - DotProduct( &A, &B)) / dotBB;
	VECTOR3		E	=	B;
	VECTOR3_MULEQU_FLOAT( &E, fTemp);
	VECTOR3_SUBEQU_VECTOR3( &E, &C);
	VECTOR3_ADDEQU_VECTOR3( &E, &A);

	VECTOR3		F	=	B;
	VECTOR3_MULEQU_FLOAT( &F, DotProduct(&B, &D)/dotBB);
	VECTOR3_SUBEQU_VECTOR3( &F, &D);

	// a*t^2 + b*t + c = 0
	float	a	=	DotProduct( &F, &F);
	float	b	=	2.0f * DotProduct( &E, &F);
	float	c	=	DotProduct( &E, &E)	-	pIn->fRadius * pIn->fRadius;

	float	det	=	b*b - 4.0f*a*c;					// 판별식.

	// 판별식 < 0
	if( det < -0.00001f)
	{
		// 만나지 않지.? 절대로.
		return	FALSE;
	}

	// 판별식 = 0
	if( det >= -0.00001f && det <= 0.00001f)
	{
		t	=	b / (-2.0f * a);
	}

	// 판별식 > 0
	if( det > 0.00001f)
	{
		t	=	(-b - (float)sqrt(det)) / (2.0f*a);
	}

	// t가 너무 멀 경우. 
	if( t > 1.0f)
	{
		return	FALSE;
	}
	if( t < -1.0f)
	{
		return	FALSE;
	}

	// 만날때 Sphere의 중점은 어디.?
	P	=	C + D*t;

	// 만날때 Line의 위치는 어디.?
	s	=	(B*C - A*B + B*D*t) /dotBB;
	if( s < 0.0f || s > 1.0f)		// line의 위치를 벗어나면,
	{
		return	FALSE;
	}
	M	=	A + B*s;		// 직선과 만나는 부분.

	// t가 '-'일 경우 스피어의 원래 위치에서 이미 만나고 있는가를 체크한다.
	if( t < 0.0f)
	{
		float	fDist	=	VECTOR3Length( &(A-M)  );
		if( fDist > pIn->fRadius)
			return	FALSE;		
		_asm nop;
	}

	pOut->fMeetTime	=	t;
	pOut->MeetPivot	=	P;
	pOut->WhereMeet	=	M;

	return	TRUE;
}

/*
	2003.03.18 엉터리로 수식 되어있던거 고쳤다. 왜 코드가 꼬여있지.?
	2002.??.?? 최초 작성. 
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	TestMovingSphereMeetPLANE( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pTri)
{
	BOOL		bResult;
	float		fTemp;

	float		fPlaneDotVelocity;

	float		t;			// 스피어가 면에 닿을때의 시점.
	VECTOR3		X;			// 스피어가 면에 닿는 위치.
	VECTOR3		MeetPos;	// 스피어와 면이 맞닿는 점.

	VECTOR3		Velocity;
	VECTOR3_SUB_VECTOR3( &Velocity, &(pIn->To), &(pIn->From));

	PLANE	p;
//	MakePLANE( &p, &(pTri[0]) , &(pTri[1]), &(pTri[2]));
	CalcPlaneEquation( &p, pTri);


	// 평행하거나, 스피어의 진행 방향과 면의 노말이 같은 방향이면 영향을 못 미친다.
	fPlaneDotVelocity	=	DotProduct( &(p.v3Up), &Velocity);
	if( fPlaneDotVelocity >= 0.0f)
	{
		// 안만난다 리턴.
		return	FALSE;
	}

	// 스피어의 원점	: X = F + (T-F)*t
	// 평면에서 스피어 원점까지의 거리.		: X*N + d = r  (r은 스피어 반지름.)
	// t = -1*(N*F+d-r)/(N*(T-F))
	t	=	DotProduct( &(p.v3Up), &(pIn->From));
	t	=	pIn->fRadius - (t + p.D);
	t	=	t / fPlaneDotVelocity;//DotProduct( &(p.v3Up), &Velocity);
	// X = F + (T-F)*t
	VECTOR3_MUL_FLOAT( &X, &Velocity, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &(pIn->From));


	if( t > 1.0f)
	{
		return	FALSE;
	}

	if( t < -1.0f)	//	이런 경우는,
	{
		//	그냥 리턴 해도 될까.?
		//	계산이 정확하다면 당연하겠지.?
		return	FALSE;

		//	'-1'이면 괜찮은건가.? 더 높게.?
	}

	// 여기서 0에 가까운 t값은 그냥 0으로 처리 하는게 좋지 않을까.?
	

	// 스피어와 평면이 만날때 서로 맞닿은 점은.?
	// X에서 평면노말의 반대방향으로 반지름만큼 간 점.
	// MeetPos	=	X + N*(-1*fRadius)
	MeetPos	=	p.v3Up;
	VECTOR3_MULEQU_FLOAT( &MeetPos, pIn->fRadius * -1.0f);
	VECTOR3_ADDEQU_VECTOR3( &MeetPos, &X);

	// 그 만나는 점이 삼각형 안에 있는가.?
//	VECTOR2		v2Meet;
	bResult	=	IsTriMeetVertex(pTri, &MeetPos);
	if( bResult	== FALSE)
		return	FALSE;

	// 만약 float의 오차에 의해 이미 만나고 있는 점으로 판별될 경우를 가려서 처리한다.
	if( t < 0.0f)
	{
		// 만약 이미 만나고 있는 경우라면 스피어가 움직이기 전의 위치를 리턴한다.
		fTemp	=	DotProduct( &(pIn->From), &(p.v3Up)) + p.D;
		if( fTemp <= 0.0f )	// 완전히 벗어났다면,
			return	FALSE;

		pOut->fMeetTime	=	0.0f;
		pOut->MeetPivot	=	pIn->From;
		pOut->WhereMeet	=	pIn->From	+	(p.v3Up * -1.0f) * pIn->fRadius;

		CheckVectorArray( &(pOut->MeetPivot), 1);


		return	TRUE;

	}

	// 이까지 오면 삼각형을 이루는 평면과 만난다.
	// pOut 정리하고 리턴.
	pOut->fMeetTime	=	t;
	pOut->MeetPivot	=	X;
	pOut->WhereMeet	=	MeetPos;
	// pOut->MeetPlane은 나중에 일괄적으로 계산.

//	VECTOR3	vTemp	=	pIn->From	+	(p.v3Up * -1.0f) * pIn->fRadius;
		CheckVectorArray( &(pOut->MeetPivot), 1);

	// 리턴.
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	움직이는 스피어가 점과 만나는가.?
	
	점은 P, 
	스펴의 중점은 X = A + B*t;	로 정의.
	PX의 길이는 r.

*/
BOOL TestMovingSphereMeetVertex( COLLISION_OUT* pOut, COLLISION_IN* pIn, VECTOR3* pP)
{
	VECTOR3		A	=	pIn->From;								// 스펴 프럼.
	VECTOR3		B;	
	VECTOR3_SUB_VECTOR3( &B, &(pIn->To), &(pIn->From));			// 스펴 속도벡터.
	VECTOR3		X;												// 스펴가 점과 만날때의 위치.
	float		t;												// 만나는 시점.
	// X = A + B*t

	VECTOR3		P	=	*pP;		// 타겟 점.

	VECTOR3		PA;
	VECTOR3_SUB_VECTOR3( &PA, &A, &P);

	float		a	=	DotProduct( &B, &B);
	float		b	=	2.0f * DotProduct( &B, &PA);
	float		c	=	DotProduct( &PA, &PA)	-	pIn->fRadius * pIn->fRadius;

	float		det	=	b*b		-	4*a*c;

	if( det <= -0.00001f)
	{
		return	FALSE;
	}
	
	t	=	(-b - (float)sqrt(det)) / (2*a);
	if( t > 1.0f)
	{
		return	FALSE;
	}

	//	X	=	A	+	B*t;
	VECTOR3_MUL_FLOAT( &X, &B, t);
	VECTOR3_ADDEQU_VECTOR3( &X, &A);

	// t가 '-'이더라도 이미 만나고 있으면 TRUE 리턴.
	VECTOR3		vTemp;
	VECTOR3_SUB_VECTOR3( &vTemp, &(pIn->From), &P);
	float	fDist	=	VECTOR3Length( &vTemp);

	if( fDist >= pIn->fRadius)
	{
		return	FALSE;
	}

	// pOut 정리.
	pOut->fMeetTime	=	t;			// 만났을때 시점.
	pOut->MeetPivot	=	X;			// 만났을때 스펴 피봇.
	pOut->WhereMeet	=	P;			// 당근 점이지.?

	return	TRUE;
}

