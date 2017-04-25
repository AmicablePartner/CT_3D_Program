#ifndef _XMLPARSE_H_
#define _XMLPARSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IXMLNode;

class IXMLParse
{
public:
	virtual BOOL Init(const char * szFileName,BOOL bOverlay = FALSE,BOOL bCreate=FALSE) =0 ; //初始化；追加方式，重写方式(bOverlay == True);
	//bCreate=True：若为追加方式找不到文件时，是否创建新文件
	virtual	BOOL Save() =0;    //存盘

	virtual IXMLNode * GetDocumentNode() =0; // 获得Document结点指针 ；其下只能添加1个根结点
	virtual ~IXMLParse()  {} ;

};

class IXMLNode
{
public:
	virtual	const char * GetAttribute(const char * key) = 0;  //获取属性值 ; NULL:无该属性
	virtual	BOOL GetAttribute(const char * key,float & value)=0;
	virtual	BOOL GetAttribute(const char * key,double & value)=0;
	virtual	BOOL GetAttribute(const char * key,int & value)=0;
	
	virtual BOOL GetAttribute ( IN const char * szKey, OUT void * pData, OUT int & nSize ) = 0;

	virtual	BOOL SetAttribute(const char * key,const char * value) = 0;  //设置属性值
	virtual BOOL SetAttribute(const char * key,float  value)=0;
	virtual BOOL SetAttribute(const char * key,double value)=0;
	virtual BOOL SetAttribute(const char * key,int  value)=0;

	virtual BOOL SetAttribute ( IN const char * szKey, IN const void * pData, IN int nSize ) = 0;

	virtual BOOL DelAttribute(const char * key) = 0;               //删除属性
	virtual	const char * GetText() = 0;                //获取文本值
	virtual BOOL SetText(const char * value) = 0;					//设置文本值

	virtual IXMLNode * AddNode(const char * name) = 0;			// 增加子结点
	virtual BOOL DeleteNode(const char * name,BOOL bAll = FALSE) = 0;  //删除子结点，bAll =True,删除同名的所有子节点

	virtual int GetChildrenCount()=0;              //获取子结点数
	virtual IXMLNode * GetFirstChildNode() = 0;   //获得第一个子结点指针
	virtual IXMLNode * GetNextSiblingNode() =0;   // 获得下一个兄弟结点指针

	virtual IXMLNode * GetChildrenNode(const char * name,BOOL bAdd = FALSE)=0; //通过名称查找第一个子结点指针，bADD=True:找不到时增加该节
	virtual IXMLNode * GetSiblingNode(const char * name)=0; //通过名称查找下一个兄弟结点指针

	virtual IXMLNode * GetParentNode() = 0; //获取父结点指针

	virtual const char * GetNodeName() = 0; //获取结点名称

	//
	virtual BOOL GetText(float & value)=0;
	virtual BOOL GetText(double & value)=0;
	virtual BOOL GetText(int & value)=0;

	virtual BOOL SetText(float  value)=0;
	virtual BOOL SetText(double  value)=0;
	virtual BOOL SetText(int  value)=0;

	virtual ~IXMLNode() {};
};

#ifdef XMLPARSE_STATICLIB

#define  XMLPARSE_API

#else

#ifdef XMLPARSE_EXPORTS
#define XMLPARSE_API __declspec(dllexport)
#else
#define XMLPARSE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#endif

	XMLPARSE_API  IXMLParse * XML_CreateParse();


#if !defined(XMLPARSE_STATICLIB) && defined(__cplusplus)
	}
#endif


#endif
