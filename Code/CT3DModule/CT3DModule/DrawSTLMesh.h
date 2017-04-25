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

	//获取模型对象
	Ogre::ManualObject* GetStlModePtr();
	
	//变形
	void DeformMesh(Ogre::Vector3 nCenterPos, float nPower, float nRadius);

protected:
	//初始化STL模型数据
	void InitSTLData();

	//初始化模型对象
	void InitSTLModel();

	//二进制解析STL
	void ParseByBinary();

	//ASCII码解析STL
	void ParseByASCII(std::vector<std::string>& strDataList);

	//更新模型
	void UpdateMesh(std::vector<Ogre::Vector3>& PosList, std::vector<Ogre::Vector3>& NormalList);

protected:

	//STL文件的路径
	CString		m_STLFilePath;

	//场景管理器
	Ogre::SceneManager*		m_MainSceneManager;

	//STL模型的对象
	Ogre::ManualObject*		m_ModelManual;

	//定点数据列表
	std::vector<Ogre::Vector3>	m_STLPosList;

	//法线的数据列表
	std::vector<Ogre::Vector3>	m_STLNomalList;
};

