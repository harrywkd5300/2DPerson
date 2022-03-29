#pragma once

// Typedef
typedef unsigned int			_uint;
typedef signed int				_int;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;	

	D3DXMATRIX matWorld;

	TCHAR  strName[MIDDLE_STR];

	BYTE byOption;
	BYTE byDrawID;

	tagInfo(void){}

	tagInfo(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vSize)
		:vPos(_vPos)
		,vSize(_vSize)
	{}
	tagInfo(D3DXVECTOR3 _vPos)
		:vPos(_vPos)
	{}
}INFO, *PINFO, *pINFO;


typedef struct tagPlayer
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;

	D3DXMATRIX matWorld;


	wstring wstrStateKey;
	wstring wstrObjectKey;
	//TCHAR  wstrStateKey[MIDDLE_STR];
	//TCHAR  wstrObjectKey[MIDDLE_STR];

	BYTE byDrawID;

}PLAYER;


typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;			
	D3DXIMAGE_INFO		ImageInfo;
}TEX_INFO;



typedef struct tagStringCompare
{
	const TCHAR* m_pString;

	template<typename T>
	bool operator () (T rData)
	{
		return !lstrcmp(rData.first, m_pString);
	}

	tagStringCompare(const TCHAR* pKey)
		:m_pString(pKey){}
}STRING_COMPARE;


struct DeleteMap 
{
	template<typename T>
	void operator () (T& Temp)
	{
		if(Temp.second)
		{
			delete [] Temp.first;

			delete Temp.second;
			Temp.second = NULL;
		}
	}
};

struct DeleteObj
{
	template<typename T>
	void operator() (T& rData)
	{
		if (rData)
		{
			safeDelete(rData);
		}
	}
};



typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;

	BYTE byOption;
	BYTE byDrawID;

}TILE, *PTILE, *pTILE;


typedef struct tagObject
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;

	D3DXMATRIX matWorld;

	int byDrawID;
	int byOption;

	TCHAR  wstrObjKey[MIDDLE_STR];
	TCHAR  wstrStateKey[MIDDLE_STR];
	TCHAR  wstrImagePath[MAX_STR];
	
}OBJECT;

typedef struct tagUnitData 
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;

	D3DXMATRIX matWorld;

	TCHAR  wstrName[MIDDLE_STR];
	TCHAR  wstrObjKey[MIDDLE_STR];
	TCHAR  wstrStateKey[MIDDLE_STR];
	TCHAR  wstrImagePath[MAX_STR];
	int		iAtt;
	int		iDef;
	int		iHp;
	int		iLevel;
	int		iMoney;
	int		byIndex;

	BYTE byOption;
	BYTE byDrawID;

}UNIT_DATA;


typedef struct tagImageInfo
{
	wstring		wstrObjKey;
	wstring		wstrStateKey;
	wstring		wstrPath;
	int			iCount;

	tagImageInfo()
		:wstrObjKey(L""),
		 wstrStateKey(L""),
		 wstrPath(L""),
		 iCount(0)
	{

	}

}IMAGE_PATH;

typedef struct tagFrame
{
	float fIndex;
	float fCount;
	float fMax;

	tagFrame(float _fIndex = 0.f, float _fCount = 0.f, float _fMax = 0.f)
		:fIndex(_fIndex), fCount(_fCount), fMax(_fMax)
	{}

}FRAME, *PFRAME;


typedef struct tagItemPath
{
	wstring wstrObjKey;
	wstring wstrImagePath;
}ITEM_PATH;

typedef struct tagItemData
{
	wstring wstrName;
	wstring wstrObjKey;
	wstring wstrImagePath;
	int		iAtt;
	int		iArmor;
	int		iHpRegen;
	int		iPrice;
	int		iBitMapNum;		//툴에서만 사용할 변수.
	//eItemType eType;

}ITEM_DATA;

typedef struct tagAStarNode
{
	float			fCost;
	int				iIndex;
	int				iWay;
	tagAStarNode*	pParent;
}NODE;


#ifdef _PROJECT_MFC_

inline wstring RelativePath(wstring szFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);

	PathRelativePathTo(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY, 
					   szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;

}

static int DirectoryFileCount(const wstring& wstrPath)
{
	wstring wstrTemp = wstrPath + L"\\*.png";

	CFileFind	Find;
	Find.FindFile(wstrTemp.c_str());

	int iContinue  = 1;
	int iFilecount = 0;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		if(!Find.IsDirectory())
			++iFilecount;
	}
	return iFilecount;
}

static void DirectoryInfoExtraction(const wstring& wstrPath, 
									list<IMAGE_PATH*>& rPathInfoList)

{
	CFileFind Find;
	Find.FindFile(wstrPath.c_str());

	int iContinue = 1;
	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		if(Find.IsDirectory())
		{
			DirectoryInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathInfoList);
		}
		else
		{
			if(Find.IsSystem())
				continue;

			IMAGE_PATH* pImagePath = new IMAGE_PATH;

			TCHAR szPathBuffer[MAX_PATH] = L"";

			lstrcpy(szPathBuffer, Find.GetFilePath().operator LPCWSTR());
			//\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack0.png
			//	                                    (                  )->삭제하기!!               
			PathRemoveFileSpec(szPathBuffer);  

			pImagePath->iCount = DirectoryFileCount(szPathBuffer);

			wstring wstrImageName = Find.GetFileTitle().operator LPCWSTR();
			
			wstrImageName = wstrImageName.substr(0, wstrImageName.size()-1) + L"%d.png";
		
			TCHAR szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().operator LPCWSTR());
		
			PathRemoveFileSpec(szPath);

			PathCombine(szPath, szPath, wstrImageName.c_str());

			wstring wstrRelativePath = RelativePath(szPath);

			pImagePath->wstrPath = wstrRelativePath;

			PathRemoveFileSpec(szPath);

			pImagePath->wstrStateKey = PathFindFileName(szPath);

			PathRemoveFileSpec(szPath);

			pImagePath->wstrObjKey = PathFindFileName(szPath);

			rPathInfoList.push_back(pImagePath);

			Find.FindFile(Find.GetFilePath());

			Find.FindNextFile();
		}

	}
};


#endif
