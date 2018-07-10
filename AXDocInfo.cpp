// AXDocInfo.cpp
//
//Copyright Andrew A. Garbuzov

#include "stdafx.h"
#include "AXDocContainer.h"

#include "AXDocContainerDoc.h"

#include <comcat.h>

CString AFXAPI AfxStringFromCLSID(REFCLSID rclsid);

IMPLEMENT_DYNAMIC(CAXDocContainerTemplate,CMultiDocTemplate);

void CAXDocInfoArray::Clear()
{
	for(int i=0; i<GetSize(); i++)
		delete GetAt(i);

	RemoveAll();
}



void CAXDocInfoArray::LoadFromRegistry()
{
	//Remove any data from the array
	Clear();

	ICatInformation* pCatInformation = NULL ;
	IEnumCLSID* pEnumCLSID = NULL;

	CATID			rgcatid[1];
	rgcatid[0] =	CATID_DocObject;

	CLSID			clsid;
	ULONG			ulFetched=0;
	CString			strCLSID;
	TCHAR			szInfo[80];
	long			lInfoLength;

	
	

	HKEY			hCLSIDKey=NULL;
	HKEY			hDocInfoKey=NULL;

	HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                               NULL,
                               CLSCTX_INPROC_SERVER,
                               IID_ICatInformation,
                               (void**)&pCatInformation);

	if(FAILED(hr))
		goto lblCleanUp;

	//Enum all doc object classes
	hr = pCatInformation->EnumClassesOfCategories(1,rgcatid,0,NULL,&pEnumCLSID);
	if(FAILED(hr))
		goto lblCleanUp;

	//Needed to get information from the registry
	if(RegOpenKeyEx(HKEY_CLASSES_ROOT,_T("CLSID"), 0, KEY_READ,
               &hCLSIDKey) != ERROR_SUCCESS)
	{
		goto lblCleanUp;
	}

	
	//Walking over all the doc object classes
	while((hr=pEnumCLSID->Next(1,&clsid,&ulFetched))==S_OK && ulFetched==1)
	{
		strCLSID=AfxStringFromCLSID(clsid);

		//Reading the name of the document
		lInfoLength=sizeof(szInfo)/sizeof(TCHAR);
		if(RegQueryValue( hCLSIDKey, strCLSID, szInfo, &lInfoLength) != ERROR_SUCCESS)
			continue;
		
		//Opening the key for the document
		if(RegOpenKeyEx(hCLSIDKey,strCLSID, 0, KEY_READ,&hDocInfoKey) != ERROR_SUCCESS)
			continue;
		
		//Reading the information about the document
		CString	strWindowTitle;
		CString	strDocName=szInfo;
		CString	strFileNewName=szInfo;
		CString	strFilterName;
		CString	strFilterExt;
		CString strRegFileTypeId;
		CString strRegFileTypeName;

		lInfoLength=sizeof(szInfo)/sizeof(TCHAR);
		if(RegQueryValue( hDocInfoKey, _T("DefaultExtension"), szInfo, &lInfoLength) == ERROR_SUCCESS)
		{
			CString strValue=szInfo;
			int nCommaPos=strValue.Find(_T(','));
			if(nCommaPos!=-1)
			{
				strFilterExt=strValue.Left(nCommaPos);
				strFilterName=strValue.Right(strValue.GetLength()-nCommaPos-1);
			}
			else
				strFilterExt=strValue;
		}

		RegCloseKey(hDocInfoKey);
		hDocInfoKey=NULL;
		
		CAXDocInfo* pInfo=new CAXDocInfo;
		pInfo->m_clsid=clsid;
		pInfo->m_strDocStrings=strWindowTitle + _T('\n') +
								strDocName + _T('\n') +
								strFileNewName + _T('\n') +
								strFilterName + _T('\n') +
								strFilterExt + _T('\n') +
								strRegFileTypeId + _T('\n') +
								strRegFileTypeName;
		Add(pInfo);
	}
	

lblCleanUp:
	if(hCLSIDKey!=NULL)
		RegCloseKey(hCLSIDKey);

	if(pEnumCLSID)
		pEnumCLSID->Release();

	if(pCatInformation)
		pCatInformation->Release();
}