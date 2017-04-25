#ifndef _XMLPARSE_H_
#define _XMLPARSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IXMLNode;

class IXMLParse
{
public:
	virtual BOOL Init(const char * szFileName,BOOL bOverlay = FALSE,BOOL bCreate=FALSE) =0 ; //��ʼ����׷�ӷ�ʽ����д��ʽ(bOverlay == True);
	//bCreate=True����Ϊ׷�ӷ�ʽ�Ҳ����ļ�ʱ���Ƿ񴴽����ļ�
	virtual	BOOL Save() =0;    //����

	virtual IXMLNode * GetDocumentNode() =0; // ���Document���ָ�� ������ֻ�����1�������
	virtual ~IXMLParse()  {} ;

};

class IXMLNode
{
public:
	virtual	const char * GetAttribute(const char * key) = 0;  //��ȡ����ֵ ; NULL:�޸�����
	virtual	BOOL GetAttribute(const char * key,float & value)=0;
	virtual	BOOL GetAttribute(const char * key,double & value)=0;
	virtual	BOOL GetAttribute(const char * key,int & value)=0;
	
	virtual BOOL GetAttribute ( IN const char * szKey, OUT void * pData, OUT int & nSize ) = 0;

	virtual	BOOL SetAttribute(const char * key,const char * value) = 0;  //��������ֵ
	virtual BOOL SetAttribute(const char * key,float  value)=0;
	virtual BOOL SetAttribute(const char * key,double value)=0;
	virtual BOOL SetAttribute(const char * key,int  value)=0;

	virtual BOOL SetAttribute ( IN const char * szKey, IN const void * pData, IN int nSize ) = 0;

	virtual BOOL DelAttribute(const char * key) = 0;               //ɾ������
	virtual	const char * GetText() = 0;                //��ȡ�ı�ֵ
	virtual BOOL SetText(const char * value) = 0;					//�����ı�ֵ

	virtual IXMLNode * AddNode(const char * name) = 0;			// �����ӽ��
	virtual BOOL DeleteNode(const char * name,BOOL bAll = FALSE) = 0;  //ɾ���ӽ�㣬bAll =True,ɾ��ͬ���������ӽڵ�

	virtual int GetChildrenCount()=0;              //��ȡ�ӽ����
	virtual IXMLNode * GetFirstChildNode() = 0;   //��õ�һ���ӽ��ָ��
	virtual IXMLNode * GetNextSiblingNode() =0;   // �����һ���ֵܽ��ָ��

	virtual IXMLNode * GetChildrenNode(const char * name,BOOL bAdd = FALSE)=0; //ͨ�����Ʋ��ҵ�һ���ӽ��ָ�룬bADD=True:�Ҳ���ʱ���Ӹý�
	virtual IXMLNode * GetSiblingNode(const char * name)=0; //ͨ�����Ʋ�����һ���ֵܽ��ָ��

	virtual IXMLNode * GetParentNode() = 0; //��ȡ�����ָ��

	virtual const char * GetNodeName() = 0; //��ȡ�������

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
