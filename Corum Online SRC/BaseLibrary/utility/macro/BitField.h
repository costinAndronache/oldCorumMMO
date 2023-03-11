#pragma once

#ifndef _BITFIELD_H_
#define _BITFIELD_H_

// Bit
#define BIT_CHECK( p, n )	((p) & (n))							// ��Ʈ �˻�
#define BIT_SET( p, n )		((p) |= (n))						// ��Ʈ ����
#define BIT_REMOVE( p, n )	((p) &= ~(n))						// ��Ʈ ����
#define BIT_ZERO( p )		((p) = 0)							// ��ü��Ʈ ����

// Bit fields
typedef long				BitField;
#define MYNBBY				8
#define MYNFDBITS			(sizeof(BitField) * MYNBBY)
#define MAX_BITSFIELD(x)	(((x)+((MYNFDBITS)-1))/(MYNFDBITS))

#define BITF_SET( p, n )	((p)[(n)/MYNFDBITS] |=  (1 << ((n) % MYNFDBITS)))
#define BITF_REMOVE( p, n)	((p)[(n)/MYNFDBITS] &= ~(1 << ((n) % MYNFDBITS)))
#define BITF_CHECK( p, n)	((p)[(n)/MYNFDBITS] &   (1 << ((n) % MYNFDBITS)))
#define BITF_ZERO( p )		memset( p, 0, sizeof( *p ) );

// BitField m_bfQuest[ MAX_BITSFIELD( 1000 ) ];			// BitField ���� (1000��)

// BITF_ZERO( m_bfQuest );								// BitField �ʱ�ȭ
// BITF_CHECK( m_bfQuest, 100 );						// 100��° ��Ʈ Ȯ�� (TRUE/FALSE ��ȯ)
// BITF_REMOVE( m_bfQuest, 100 );						// 100��° ��Ʈ ����
// BITF_SET( m_bfQuest, 100 );							// 100��° ��Ʈ ����

#endif /* _BITFIELD_H_ */
