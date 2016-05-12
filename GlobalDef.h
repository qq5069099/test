#ifndef __GLOBALDEF_H__
#define __GLOBALDEF_H__


//����
struct tagKind
{
    int					KindID;
    char				KindName[32];
    bool				IsOut;
    bool				AddTotal;
	int					Width;
};

//��Ŀ���
struct tagKindValue
{
    int					ValueID;
    int					KindID;
    float				Value;
};

//��Ŀ��Ŀ
struct tagSdupItem
{
    int				SdupID;
    char			SdupValue[64];
    __time64_t		AtTime;
    float			LeftValue;				//��ˮ����
    tagKindValue	* pKindValue;
	int				KindValueCount;
    tagSdupItem()
	{
		pKindValue = NULL;
		SdupID = 0;
		ZeroMemory(SdupValue, sizeof(SdupValue));
		AtTime = 0;
		LeftValue = 0.0f;
		pKindValue = NULL;
	}
	~tagSdupItem()
	{
		if (pKindValue) delete pKindValue;
		pKindValue = NULL;
	}
	void Clear()
    {
        SdupID = 0;
        ZeroMemory(SdupValue, sizeof(SdupValue));
        SdupValue[64];
        AtTime = 0;
        LeftValue = 0.0f;				//��ˮ����
		if (pKindValue)
		{
			delete [] pKindValue;
			pKindValue = NULL;
		}
		KindValueCount = 0;
	}
	void SetKindValueCount(int nKindValueCount)
	{
		if (pKindValue)
		{
			delete [] pKindValue;
			pKindValue = NULL;
		}
		KindValueCount = nKindValueCount;
		if (nKindValueCount > 0)
		{
			pKindValue = new tagKindValue[nKindValueCount];
			ZeroMemory(pKindValue, sizeof(tagKindValue) * nKindValueCount);
		}
	}
	tagSdupItem(const tagSdupItem & SdupItem)
	{
		Clear();
		SdupID = SdupItem.SdupID;
		memcpy(SdupValue, SdupItem.SdupValue, sizeof(SdupValue));
		AtTime = SdupItem.AtTime;
		LeftValue = SdupItem.LeftValue;
		KindValueCount = SdupItem.KindValueCount;
		SetKindValueCount(SdupItem.KindValueCount);
		if (KindValueCount > 0)
			memcpy(pKindValue, SdupItem.pKindValue, sizeof(tagKindValue) * KindValueCount);
	}
    tagSdupItem & operator =(const tagSdupItem & SdupItem)
	{
		Clear();
		SdupID = SdupItem.SdupID;
		memcpy(SdupValue, SdupItem.SdupValue, sizeof(SdupValue));
		AtTime = SdupItem.AtTime;
		LeftValue = SdupItem.LeftValue;
		KindValueCount = SdupItem.KindValueCount;
		SetKindValueCount(SdupItem.KindValueCount);
		if (KindValueCount > 0)
			memcpy(pKindValue, SdupItem.pKindValue, sizeof(tagKindValue) * KindValueCount);

        return *this;
	}
};

#define			CountArray(ar)			(sizeof(ar) / sizeof(ar[0]))		//����Ԫ������


enum eLoadType { lTypeIn, lTypeOut, lTypeAll };

#endif // __GLOBALDEF_H__