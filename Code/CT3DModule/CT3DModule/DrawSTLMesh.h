#pragma once

struct StlPoint
{
	float	x;
	float	y;
	float	z;
};

struct StlFace
{
	StlPoint	nomal;
	StlPoint	FristPoint;
	StlPoint	SecondPoint;
	StlPoint	ThridPoint;
	BYTE		emptyValue1;
	BYTE		emptyValue2;
};

class CDrawSTLMesh
{
public:
	CDrawSTLMesh(CString strPath, Ogre::SceneManager* MainMgr);
	~CDrawSTLMesh();

	//��ȡģ�Ͷ���
	Ogre::ManualObject* GetStlModePtr();
	
	//����
	void DeformMesh(Ogre::Vector3 nCenterPos, float nPower, float nRadius);

protected:
	//��ʼ��STLģ������
	void InitSTLData();

	//��ʼ��ģ�Ͷ���
	void InitSTLModel();

	//�����ƽ���STL
	void ParseByBinary();

	//ASCII�����STL
	void ParseByASCII(std::vector<std::string>& strDataList);

	//����ģ��
	void UpdateMesh(std::vector<Ogre::Vector3>& PosList, std::vector<Ogre::Vector3>& NormalList);

protected:

	//STL�ļ���·��
	CString		m_STLFilePath;

	//����������
	Ogre::SceneManager*		m_MainSceneManager;

	//STLģ�͵Ķ���
	Ogre::ManualObject*		m_ModelManual;

	//���������б�
	std::vector<Ogre::Vector3>	m_STLPosList;

	//���ߵ������б�
	std::vector<Ogre::Vector3>	m_STLNomalList;
};

