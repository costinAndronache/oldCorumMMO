// MyZipArchive.cpp: implementation of the CMyZipArchive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyZipArchive.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyZipArchive::CMyZipArchive()
{
}

CMyZipArchive::~CMyZipArchive()
{
}

bool CMyZipArchive::AddNewFile( LPCTSTR lpszFilePath, LPCTSTR lpszCloneFilePath, FILETIME ftFileTime, DWORD dwFileAttr, int iLevel, ZIPCALLBACKFUN pCallback, void* pUserData, unsigned long nBufSize)
{
	if (!nBufSize)
		return false;
	
	CZipFileHeader header;
	header.SetFileName( lpszFilePath );
	if (header.GetFileNameSize() == 0)
		return false;
	if (!OpenNewFile( ftFileTime, dwFileAttr, header, iLevel))
		return false;
	
	if ( !(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY) )
	{
		CFile f;
		CFileException* e = new CFileException;
		BOOL bRet = f.Open(lpszCloneFilePath, CFile::modeRead | CFile::shareDenyWrite, e);
		e->Delete();
		if (!bRet)
			return false;
		
		DWORD iRead, iFileLength = pCallback ? f.GetLength() : 0, iSoFar = 0;
		CZipAutoBuffer buf(nBufSize);
		do
		{
			iRead = f.Read(buf, nBufSize);
			if (iRead)
			{
				WriteNewFile(buf, iRead);
				iSoFar += iRead;
				if (pCallback)
					if (!pCallback(iFileLength, iSoFar, pUserData))
						break;
			}
		}
		while (iRead == buf.GetSize());
	}
	CloseNewFile();
	return true;
}


bool CMyZipArchive::OpenNewFile( FILETIME ftFileTime, DWORD dwFileAttr, CZipFileHeader& header, int iLevel)
{
	if (IsClosed())
	{
		TRACE(_T("ZipArchive is closed.\n"));
		return false;
	}
	
	if (m_iFileOpened)
	{
		TRACE(_T("A file already opened.\n"));
		return false;
	}
	
	if (m_storage.IsSpanMode() == -1)
	{
		TRACE(_T("You cannot add the files to the existing disk spannig archive.\n"));
		return false;
	}
	
	if (GetNoEntries() ==(WORD)USHRT_MAX)
	{
		TRACE(_T("Maximum file count inside archive reached.\n"));
		return false;
	}
	
	CTime sTime( ftFileTime );
	header.SetTime( sTime );
	header.m_uExternalAttr = dwFileAttr;
			
	m_info.Init();
	m_centralDir.AddNewFile(header);

	CString szFileName = CurrentFile()->GetFileName();
	
	if (szFileName.IsEmpty())
	{
		szFileName.Format(_T("file%i"), GetNoEntries());;
		CurrentFile()->SetFileName(szFileName);
	}
	// this ensures the conversion will take place anyway (must take because we are going 
	// 	to write the local header in a moment
	m_centralDir.ConvertFileName(false, m_centralDir.m_bConvertAfterOpen);


	bool bIsDirectory = IsDirectory(CurrentFile()->m_uExternalAttr);
	bool bEncrypted = m_pszPassword.GetSize() != 0;
#ifdef _DEBUG
	if (bIsDirectory && bEncrypted)
		TRACE(_T("Warning! Encrypting a directory. Possible but pointless.\n\
		Clear the password before adding a directory.\n"));
#endif	

	
	if (!CurrentFile()->PrepareData(iLevel, m_storage.IsSpanMode() == 1, bEncrypted))
			ThrowError(ZIP_TOOLONGFILENAME);

	CurrentFile()->WriteLocal(m_storage);
	// we have written the local header, but if we keep filenames not converted
	// in memory , we have to restore the non-converted value
	if (m_centralDir.m_bConvertAfterOpen)
		CurrentFile()->SetFileName(szFileName);
	if (bEncrypted)
	{
		CZipAutoBuffer buf(ENCR_HEADER_LEN);
		// use pseudo-crc since we don't know it yet
		CryptCryptHeader((long)header.m_uModTime << 16, buf);
		m_storage.Write(buf, ENCR_HEADER_LEN, false);
	}
	
	
	m_info.m_uComprLeft = 0;
    m_info.m_stream.avail_in = (uInt)0;
    m_info.m_stream.avail_out = (uInt)m_info.m_pBuffer.GetSize();
    m_info.m_stream.next_out = (unsigned char*)(char*)m_info.m_pBuffer;
    m_info.m_stream.total_in = 0;
    m_info.m_stream.total_out = 0;
	
	if (bIsDirectory && (CurrentFile()->m_uMethod != 0))
		CurrentFile()->m_uMethod = 0;
	
	if (CurrentFile()->m_uMethod == Z_DEFLATED)
    {
        m_info.m_stream.opaque = m_bDetectZlibMemoryLeaks ? &m_list : 0;
		
        int err = deflateInit2(&m_info.m_stream, iLevel,
			Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);
		
		CheckForError(err);
    }
	m_iFileOpened = compress;
	return true;
}
