

#ifndef	_MouseLib_h_
#define	_MouseLib_h_

#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common/IGeometry.h"
#else
#include "./../4DyuchiGRX_common/IGeometry.h"
#endif

// ��ũ������ x,y��ǥ�� ���� ��ǥ�� x,0,z�� �ٲ۴�. y�� ������ 0
//	����������, ī�޶� from�� y��ǥ�� 0 �̻��϶� ���򼱺��� ���� ���� ������, �������� ���ʿ��� ������ ��ǥ�� ���´�.
VECTOR3	GetXYZFromScreenXY( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY);

// �������� VECTOR3�� ȭ����� ������� �˾Ƴ��� �Լ�.
// �ܼ��� pWhere�� Ʈ�������� ��ģ �� ����� pOutScreen�� ����Ѵ�.
// ������ pOutScreen.x�� .y�� ����, ���� ���� 0.0f, ������ �Ʒ��� ���� 1.0f �̹Ƿ�, ȭ�� �ػ󵵸� ���ؾ� ȭ����� ��ǥ�� ���� ��
// �ִ�. ������ 0.0f�� 1.0f���̸� ����� ��� ȭ�鿡 �ȳ��´ٰ� ���� �ȴ�.
void	GetScreenXYFromXYZ( I4DyuchiGXGeometry* pGeometry, DWORD dwViewportIndex, VECTOR3* pInWhere, VECTOR3* pOutScreen);

#endif