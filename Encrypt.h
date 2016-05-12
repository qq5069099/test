#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

//////////////////////////////////////////////////////////////////////////

//MD5 加密类
class CMD5Encrypt
{
	//功能函数
public:
	//生成密文
	static void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
};

//////////////////////////////////////////////////////////////////////////

//异或加密类
class CXOREncrypt
{
	//功能函数
public:
	//生成密文
	static WORD EncryptData(LPCTSTR pszSrcData, LPTSTR pszEncrypData, WORD wSize);
	//解开密文
	static WORD CrevasseData(LPCTSTR pszEncrypData, LPTSTR pszSrcData, WORD wSize);
};

//////////////////////////////////////////////////////////////////////////


#endif // __ENCRYPT_H__
