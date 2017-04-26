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
	int nFaceCount = 0;//�����
	//while (std::getline(ifs, line))
	//{
	//	char *Del = " ";
	//	char buff[60];
	//	char* Next_token = NULL;
	//	strcpy_s(buff, line.c_str());

	//	if (strcmp(line.c_str(), "endfacet") == 0)
	//	{
	//		nFaceCount++;
	//		//�����̫��ͷ����洢
	//		if (nFaceCount >= 100000)
	//		{
	//			ParseByASCII(Tken);
	//			nFaceCount = 0;
	//			Tken.clear();
	//			OutputDebugString("�Ѿ�10�������!\n");
	//		}
	//	}

	//	char* token = strtok_s(buff, Del, &Next_token);  //�ֽ��ַ���Ϊһ���Ǵ�
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
		output.Format("STL�ļ���ʧ��!%d\n", e.m_cause);
		OutputDebugString(output);
		return;
	}
	//����λ��
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
			//�����̫��ͷ����洢
			if (nFaceCount >= 100000)
			{
				ParseByASCII(Tken);
				nFaceCount = 0;
				Tken.clear();
				OutputDebugString("�Ѿ�10�������!\n");
				CString output;
				output.Format("��ǰ����:%0.2f,����ʱ:%d\n", CurPos * 1.0 / nEndPos * 100, GetTickCount() - StartTime);
				OutputDebugString(output);
			}
		}
		char *Del = " ";
		char buff[256] = {0};
		char* Next_token = NULL;
		strcpy_s(buff, strLine.GetString());

		char* token = strtok_s(buff, Del, &Next_token);  //�ֽ��ַ���Ϊһ���Ǵ�
		while (token) {
			Tken.push_back(token);
			token = strtok_s(NULL, Del, &Next_token);
		}
	}
	mFile.Close();

	//���û�н��������ʹ�ö�������ʽ����
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
	//������STL�ļ��ù̶����ֽ���������������Ƭ�ļ�����Ϣ��
	CFile mFile;
	CFileException e;
	if (!mFile.Open(m_STLFilePath, CFile::modeRead | CFile::typeBinary, &e))
	{
		CString output;
		output.Format("STL�ļ���ʧ��!%s\n", e.m_cause);
		OutputDebugString(output);
		return;
	}
	//�ļ���ʼ��80���ֽ����ļ�ͷ�����ڴ����ļ�����
	char strHead[80];
	mFile.Read(strHead, 80);

	//�������� 4 ���ֽڵ�����������ģ�͵�������Ƭ������
	char strDataCount[4];
	int nDataCount;
	mFile.Read(&nDataCount, 4);

	//int nDataCount = atoi(strDataCount);

	//�����������ÿ��������Ƭ�ļ�����Ϣ��ÿ��������Ƭռ�ù̶���50���ֽڣ������� :
	StlFace* CurDataBuf = new StlFace[nDataCount];
	mFile.Read(CurDataBuf, nDataCount * sizeof(StlFace));

	for (size_t i = 0; i < nDataCount; i++)
	{
		Ogre::Vector3 CurNomal, CurPoint1, CurPoint2, CurPoint3;
		float* CurData = (float*)CurDataBuf;
		//3��4�ֽڸ�����(����Ƭ�ķ�ʸ��)
		CurNomal.x = CurDataBuf[i].nomal.x;
		CurNomal.y = CurDataBuf[i].nomal.y;
		CurNomal.z = CurDataBuf[i].nomal.z;
		//3��4�ֽڸ�����(1�����������)
		CurPoint1.x = CurDataBuf[i].FristPoint.x;
		CurPoint1.y = CurDataBuf[i].FristPoint.y;
		CurPoint1.z = CurDataBuf[i].FristPoint.z;

		//3��4�ֽڸ�����(2�����������)
		CurPoint2.x = CurDataBuf[i].SecondPoint.x;
		CurPoint2.y = CurDataBuf[i].SecondPoint.y;
		CurPoint2.z = CurDataBuf[i].SecondPoint.z;

		//3��4�ֽڸ�����(3�����������)��
		CurPoint3.x = CurDataBuf[i].ThridPoint.x;
		CurPoint3.y = CurDataBuf[i].ThridPoint.y;
		CurPoint3.z = CurDataBuf[i].ThridPoint.z;

		//������Ƭ�����2���ֽ���������������Ƭ��������Ϣ��
		
		//�������Ϊ�˶�Ӧ��������������
		m_STLNomalList.push_back(CurNomal);
		m_STLNomalList.push_back(CurNomal);
		m_STLNomalList.push_back(CurNomal);
		m_STLPosList.push_back(CurPoint1);
		m_STLPosList.push_back(CurPoint2);
		m_STLPosList.push_back(CurPoint3);
	}



	//һ������������STL�ļ��Ĵ�СΪ��������Ƭ������ 50�ټ���84���ֽڡ�
}

void CDrawSTLMesh::ParseByASCII(std::vector<std::string>& strDataList)
{
	//solidfilenamestl//�ļ�·�����ļ���
	//facetnormalxyz//������Ƭ��������3������ֵ
	//outerloop
	//vertexxyz//������Ƭ��һ����������
	//vertexxyz//������Ƭ�ڶ�����������
	//vertexxyz//������Ƭ��������������
	//endloop
	//endfacet//���һ��������Ƭ����

	//......//����facet

	//endsolidfilenamestl//����STL�ļ��������

	for (int i = 0;i < strDataList.size();++i)
	{
		if (strcmp(strDataList[i].c_str(), "vertex") == 0)  //�ж϶�ȡ���Ƿ�Ϊ������Ϣ
		{
			double x = atof(strDataList[i + 1].c_str());  //���ַ���ת���ɸ�����
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
			double x = atof(strDataList[i + 1].c_str());  //���ַ���ת���ɸ�����
			double y = atof(strDataList[i + 2].c_str());
			double z = atof(strDataList[i + 3].c_str());
			Ogre::Vector3 CurPos;
			CurPos.x = x;
			CurPos.y = y;
			CurPos.z = z;
			//�������Ϊ�˶�Ӧ��������������
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
	//����ԭʼ������
	std::vector<Ogre::Vector3> BasePosList = m_STLPosList;
	std::vector<Ogre::Vector3> BaseNormalList = m_STLNomalList;

	//������ΰ뾶��ƽ������Ӱ����η�Χ��ƽ�����Ϳ���
	float fSqrRadius = nRadius * nRadius;

	//��ƽ��������Ϊ������
	Ogre::Vector3 AverageNormal = Ogre::Vector3::ZERO;

	//������������ͷ������飬����ƽ������
	for (int i = 0; i < BasePosList.size(); ++i)
	{
		//��������ƽ������������ײ�㣩
		float SqrMagnitude = (BasePosList[i] - nCenterPos).squaredLength();
		//���ж���ɸѡ�����������ֱ�Ӻ��Ըõ�,���账��
		if (SqrMagnitude > fSqrRadius)
		{
			continue;
		}

		//��Ч����֮�ڣ����д���
		//������룬��ƽ��(��������ײ��)
		float CurDis = Ogre::Math::Sqrt(SqrMagnitude);

		//����Ȩ������(ʹ�����Ա仯ģʽ)
		float nFallOff = Ogre::Math::Clamp(1.0f - CurDis / nRadius, 0.0f, 1.0f);

		AverageNormal += nFallOff * BaseNormalList[i];
	}

	//��ƽ�����ߵ�λ��,��Ϊֻ�з��������,�������
	AverageNormal.normalise();

	//������������,�޸Ķ�������
	for (int i = 0; i < BasePosList.size(); i++)
	{
		//��������ƽ������������ײ�㣩
		float SqrMagnitude = (BasePosList[i] - nCenterPos).squaredLength();
		//���ж���ɸѡ�����������ֱ�Ӻ��Ըõ�,���账��
		if (SqrMagnitude > fSqrRadius)
		{
			continue;
		}

		//��Ч����֮�ڣ����д���
		//������룬��ƽ��(��������ײ��)
		float CurDis = Ogre::Math::Sqrt(SqrMagnitude);

		//���ڸ�˹ģ���㷨������仯����
		float nFallOff = Ogre::Math::Clamp(Ogre::Math::Pow(360.f, -Ogre::Math::Pow(CurDis / nRadius, 2.5f) - 0.01f), 0.0f, 1.0f);

		//�����޸ģ����ݵ�Ч��(��ƽ�����ߵķ�����������
		BasePosList[i] -= AverageNormal * nFallOff * nPower;
	}

	//������ģ��
	UpdateMesh(BasePosList, BaseNormalList);
}
