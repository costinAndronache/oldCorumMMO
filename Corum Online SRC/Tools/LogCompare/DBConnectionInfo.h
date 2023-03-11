#pragma once

class CDBConnectionInfo
{
public:
	enum	CDBConnectionInfo_Member
	{
		CDBDM_NAME = 0,
		CDBDM_ADDRESS,
		CDBDM_ACCOUNT,
		CDBDM_PASSWORD,
		CDBDM_MAX
	};
	__inline string&	GetStr( UINT uIndex )
	{
		ASSERT( uIndex < CDBDM_MAX );

		return m_strInfo[uIndex];
	}
	
	__inline string&	GetDBName()
	{
		return m_strInfo[CDBDM_NAME];
	}

	__inline string&	GetDBAddress()
	{
		return m_strInfo[CDBDM_ADDRESS];
	}

	__inline string&	GetAccount()
	{
		return m_strInfo[CDBDM_ACCOUNT];
	}

	__inline string&	GetPassword()
	{
		return m_strInfo[CDBDM_PASSWORD];
	}

	size_t HS_MBTOWS( string&	str, wstring &wstrTgt )
	{
		const size_t src_size = str.size();
		static	wchar_t	buffer[0xff];
		if(0 < src_size)
		{
			size_t size = mbstowcs(  buffer,   str.c_str(),   src_size );

			buffer[src_size] = 0;
			wstrTgt = buffer;

			return size;
		} // if()
		return 0;
	}

	size_t HS_MBTOWS( UINT uIndex, wstring &wstrTgt )
	{
		return HS_MBTOWS( GetStr(uIndex), wstrTgt );
	}

private:	
	string	m_strInfo[CDBDM_MAX];
};
