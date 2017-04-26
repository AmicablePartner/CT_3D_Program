#include "stdafx.h"
#include "DrawSTLMesh.h"


CDrawSTLMesh::CDrawSTLMesh(CString strPath, Ogre::SceneManager* MainMgr)
{
	m_STLFilePath = strPath;
	m_MainSceneManager = MainMgr;

	InitSTLData();

	InitSTLModel();
}


CDrawSTLMesh::~CDrawSTLMesh()
{
	m_ModelManual->clear();
	delete m_ModelManual;
	m_STLNomalList.clear();
	m_STLPosList.clear();
}

Ogre::ManualObject * CDrawSTLMesh::GetStlModePtr()
{
	return m_ModelManual;
}

void CDrawSTLMesh::InitSTLData()
{
	//std::ifstream ifs;
	//ifs.open(m_STLFilePath.GetString());
	std::string line;
	std::vector<std::string> Tken;
	int nFaceCount = 0;//面计数
	//while (std::getline(ifs, line))
	//{
	//	char *Del = " ";
	//	char buff[60];
	//	char* Next_token = NULL;
	//	strcpy_s(buff, line.c_str());

	//	if (strcmp(line.c_str(), "endfacet") == 0)
	//	{
	//		nFaceCount++;
	//		//如果面太多就分批存储
	//		if (nFaceCount >= 100000)
	//		{
	//			ParseByASCII(Tken);
	//			nFaceCount = 0;
	//			Tken.clear();
	//			OutputDebugString("已经10万个面了!\n");
	//		}
	//	}

	//	char* token = strtok_s(buff, Del, &Next_token);  //分解字符串为一组标记串
	//	while (token) {
	//		Tken.push_back(token);
	//		token = strtok_s(NULL, Del, &Next_token);
	//	}
	//}

	//ifs.close();

	DWORD StartTime = GetTickCount();
	CStdioFile mFile;
	CFileException e;
	if (!mFile.Open(m_STLFilePath, CFile::modeRead, &e))
	{
		CString output;
		output.Format("STL文件打开失败!%d\n", e.m_cause);
		OutputDebugString(output);
		return;
	}
	//最后的位置
	mFile.SeekToEnd();
	DWORD nEndPos = mFile.GetPosition();
	mFile.SeekToBegin();
	CString strLine;
	while (mFile.ReadString(strLine))
	{
		DWORD CurPos = mFile.GetPosition();
		if (strLine.CompareNoCase("endfacet") == 0)
		{
			nFaceCount++;
			//如果面太多就分批存储
			if (nFaceCount >= 100000)
			{
				ParseByASCII(Tken);
				nFaceCount = 0;
				Tken.clear();
				OutputDebugString("已经10万个面了!\n");
				CString output;
				output.Format("当前进度:%0.2f,已用时:%d\n", CurPos * 1.0 / nEndPos * 100, GetTickCount() - StartTime);
				OutputDebugString(output);
			}
		}
		char *Del = " ";
		char buff[256] = {0};
		char* Next_token = NULL;
		strcpy_s(buff, strLine.GetString());

		char* token = strtok_s(buff, Del, &Next_token);  //分解字符串为一组标记串
		while (token) {
			Tken.push_back(token);
			token = strtok_s(NULL, Del, &Next_token);
		}
	}
	mFile.Close();

	//如果没有解析出点就使用二进制形式解析
	//if (Tken.size() == 0)
	{
		//ParseByBinary();
	}
	//else
	//{
		ParseByASCII(Tken);
	//}

}

void CDrawSTLMesh::InitSTLModel()
{
	CString CurName;
	CurName.Format("MySTLManual_%d", this);
	m_ModelManual = m_MainSceneManager->createManualObject(CurName.GetString());
	m_ModelManual->setDynamic(true);
	m_ModelManual->estimateVertexCount(m_STLPosList.size() + 2);
	m_ModelManual->estimateIndexCount(m_STLPosList.size() + 2);

	m_ModelManual->clear();

	m_ModelManual->begin("CT/3DBody", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	
	for (size_t i = 0; i < m_STLNomalList.size() / 3; ++i)
	{
		Ogre::Vector3 CurPos = m_STLPosList[i * 3 + 2];
		m_ModelManual->position(CurPos);
		m_ModelManual->normal(m_STLNomalList[i * 3 + 2]);
		m_ModelManual->index(i * 3);
		m_ModelManual->textureCoord(CurPos.x, CurPos.z);

		Ogre::Vector3 CurPos1 = m_STLPosList[i * 3 + 1];
		m_ModelManual->position(CurPos1);
		m_ModelManual->normal(m_STLNomalList[i * 3 + 1]);
		m_ModelManual->index(i * 3 + 1);
		m_ModelManual->textureCoord(CurPos1.x, CurPos1.z);

		Ogre::Vector3 CurPos2 = m_STLPosList[i * 3];
		m_ModelManual->position(CurPos2);
		m_ModelManual->normal(m_STLNomalList[i * 3]);
		m_ModelManual->index(i * 3 + 2);
		m_ModelManual->textureCoord(CurPos2.x, CurPos2.z);
	}

	m_ModelManual->end();
}

void CDrawSTLMesh::ParseByBinary()
{
	//二进制STL文件用固定的字节数来给出三角面片的几何信息。
	CFile mFile;
	CFileException e;
	if (!mFile.Open(m_STLFilePath, CFile::modeRead | CFile::typeBinary, &e))
	{
		CString output;
		output.Format("STL文件打开失败!%s\n", e.m_cause);
		OutputDebugString(output);
		return;
	}
	//文件起始的80个字节是文件头，用于存贮文件名；
	char strHead[80];
	mFile.Read(strHead, 80);

	//紧接着用 4 个字节的整数来描述模型的三角面片个数，
	char strDataCount[4];
	int nDataCount;
	mFile.Read(&nDataCount, 4);

	//int nDataCount = atoi(strDataCount);

	//后面逐个给出每个三角面片的几何信息。每个三角面片占用固定的50个字节，依次是 :
	StlFace* CurDataBuf = new StlFace[nDataCount];
	mFile.Read(CurDataBuf, nDataCount * sizeof(StlFace));

	for (size_t i = 0; i < nDataCount; i++)
	{
		Ogre::Vector3 CurNomal, CurPoint1, CurPoint2, CurPoint3;
		float* CurData = (float*)CurDataBuf;
		//3个4字节浮点数(角面片的法矢量)
		CurNomal.x = CurDataBuf[i].nomal.x;
		CurNomal.y = CurDataBuf[i].nomal.y;
		CurNomal.z = CurDataBuf[i].nomal.z;
		//3个4字节浮点数(1个顶点的坐标)
		CurPoint1.x = CurDataBuf[i].FristPoint.x;
		CurPoint1.y = CurDataBuf[i].FristPoint.y;
		CurPoint1.z = CurDataBuf[i].FristPoint.z;

		//3个4字节浮点数(2个顶点的坐标)
		CurPoint2.x = CurDataBuf[i].SecondPoint.x;
		CurPoint2.y = CurDataBuf[i].SecondPoint.y;
		CurPoint2.z = CurDataBuf[i].SecondPoint.z;

		//3个4字节浮点数(3个顶点的坐标)个
		CurPoint3.x = CurDataBuf[i].ThridPoint.x;
		CurPoint3.y = CurDataBuf[i].ThridPoint.y;
		CurPoint3.z = CurDataBuf[i].ThridPoint.z;

		//三角面片的最后2个字节用来描述三角面片的属性信息。
		
		//添加三个为了对应三角形三个顶点
		m_STLNomalList.push_back(CurNomal);
		m_STLNomalList.push_back(CurNomal);
		m_STLNomalList.push_back(CurNomal);
		m_STLPosList.push_back(CurPoint1);
		m_STLPosList.push_back(CurPoint2);
		m_STLPosList.push_back(CurPoint3);
	}



	//一个完整二进制STL文件的大小为三角形面片数乘以 50再加上84个字节。
}

void CDrawSTLMesh::ParseByASCII(std::vector<std::string>& strDataList)
{
	//solidfilenamestl//文件路径及文件名
	//facetnormalxyz//三角面片法向量的3个分量值
	//outerloop
	//vertexxyz//三角面片第一个顶点坐标
	//vertexxyz//三角面片第二个顶点坐标
	//vertexxyz//三角面片第三个顶点坐标
	//endloop
	//endfacet//完成一个三角面片定义

	//......//其他facet

	//endsolidfilenamestl//整个STL文件定义结束

	for (int i = 0;i < strDataList.size();++i)
	{
		if (strcmp(strDataList[i].c_str(), "vertex") == 0)  //判断读取的是否为顶点信息
		{
			double x = atof(strDataList[i + 1].c_str());  //把字符串转化成浮点数
			double y = atof(strDataList[i + 2].c_str());
			double z = atof(strDataList[i + 3].c_str());
			Ogre::Vector3 CurPos;
			CurPos.x = x;
			CurPos.y = y;
			CurPos.z = z;
			m_STLPosList.push_back(CurPos);
		}
		else if (strcmp(strDataList[i].c_str(), "normal") == 0)
		{
			double x = atof(strDataList[i + 1].c_str());  //把字符串转化成浮点数
			double y = atof(strDataList[i + 2].c_str());
			double z = atof(strDataList[i + 3].c_str());
			Ogre::Vector3 CurPos;
			CurPos.x = x;
			CurPos.y = y;
			CurPos.z = z;
			//添加三个为了对应三角形三个顶点
			m_STLNomalList.push_back(CurPos);
			m_STLNomalList.push_back(CurPos);
			m_STLNomalList.push_back(CurPos);
		}
	}
}

void CDrawSTLMesh::UpdateMesh(std::vector<Ogre::Vector3>& PosList, std::vector<Ogre::Vector3>& NormalList)
{
	//m_ModelManual->begin("CT/3DBody", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	m_ModelManual->beginUpdate(0);

	for (size_t i = 0; i < NormalList.size() / 3; ++i)
	{
		Ogre::Vector3 CurPos1 = PosList[i * 3 + 2];
		m_ModelManual->position(CurPos1);
		m_ModelManual->normal(NormalList[i * 3 + 2]);
		m_ModelManual->index(i * 3);
		m_ModelManual->textureCoord(CurPos1.x, CurPos1.z);

		Ogre::Vector3 CurPos2 = PosList[i * 3 + 1];
		m_ModelManual->position(CurPos2);
		m_ModelManual->normal(NormalList[i * 3 + 1]);
		m_ModelManual->index(i * 3 + 1);
		m_ModelManual->textureCoord(CurPos2.x, CurPos2.z);

		Ogre::Vector3 CurPos3 = PosList[i * 3];
		m_ModelManual->position(CurPos3);
		m_ModelManual->normal(NormalList[i * 3]);
		m_ModelManual->index(i * 3 + 2);
		m_ModelManual->textureCoord(CurPos3.x, CurPos3.z);
	}

	m_ModelManual->end();
}

void CDrawSTLMesh::DeformMesh(Ogre::Vector3 nCenterPos, float nPower, float nRadius)
{
	//拷贝原始的数据
	std::vector<Ogre::Vector3> BasePosList = m_STLPosList;
	std::vector<Ogre::Vector3> BaseNormalList = m_STLNomalList;

	//计算变形半径的平方，它影响变形范围，平方降低开销
	float fSqrRadius = nRadius * nRadius;

	//将平均法线置为零向量
	Ogre::Vector3 AverageNormal = Ogre::Vector3::ZERO;

	//遍历顶点数组和法线数组，计算平均法线
	for (int i = 0; i < BasePosList.size(); ++i)
	{
		//计算距离的平方（顶点与碰撞点）
		float SqrMagnitude = (BasePosList[i] - nCenterPos).squaredLength();
		//进行顶点筛选，距离过大则直接忽略该点,不予处理
		if (SqrMagnitude > fSqrRadius)
		{
			continue;
		}

		//有效距离之内，进行处理
		//计算距离，开平方(顶点与碰撞点)
		float CurDis = Ogre::Math::Sqrt(SqrMagnitude);

		//计算权重因子(使用线性变化模式)
		float nFallOff = Ogre::Math::Clamp(1.0f - CurDis / nRadius, 0.0f, 1.0f);

		AverageNormal += nFallOff * BaseNormalList[i];
	}

	//将平均法线单位化,因为只有方向才有用,方便计算
	AverageNormal.normalise();

	//遍历顶点数组,修改顶点网络
	for (int i = 0; i < BasePosList.size(); i++)
	{
		//计算距离的平方（顶点与碰撞点）
		float SqrMagnitude = (BasePosList[i] - nCenterPos).squaredLength();
		//进行顶点筛选，距离过大则直接忽略该点,不予处理
		if (SqrMagnitude > fSqrRadius)
		{
			continue;
		}

		//有效距离之内，进行处理
		//计算距离，开平方(顶点与碰撞点)
		float CurDis = Ogre::Math::Sqrt(SqrMagnitude);

		//基于高斯模糊算法，计算变化因子
		float nFallOff = Ogre::Math::Clamp(Ogre::Math::Pow(360.f, -Ogre::Math::Pow(CurDis / nRadius, 2.5f) - 0.01f), 0.0f, 1.0f);

		//顶点修改，塌陷的效果(在平均法线的方向上做减法
		BasePosList[i] -= AverageNormal * nFallOff * nPower;
	}

	//最后更新模型
	UpdateMesh(BasePosList, BaseNormalList);
}
