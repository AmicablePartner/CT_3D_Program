#include "stdafx.h"
#include "Main3DWnd.h"

#include "CommShareMemoryMgr.h"

Ogre::Vector3 ConvertCStringToVector3(CString str)
{
	Ogre::Vector3 Result = Ogre::Vector3::ZERO;
	int nPos = str.Find(',');
	if (nPos >= 0)
	{
		Result.x = atof(str.Left(nPos).GetString());
		str = str.Right(str.GetLength() - nPos - 1);
		nPos = str.Find(',');
		if (nPos >= 0)
		{
			Result.y = atof(str.Left(nPos).GetString());
			Result.z = atof(str.Right(str.GetLength() - nPos - 1).GetString());
		}
	}
	return Result;
}
Ogre::Vector4 ConvertCStringToVector4(CString str)
{
	Ogre::Vector4 Result = Ogre::Vector4::ZERO;
	int nPos = str.Find(',');
	if (nPos >= 0)
	{
		Result.x = atof(str.Left(nPos).GetString());
		str = str.Right(str.GetLength() - nPos - 1);
		nPos = str.Find(',');
		if (nPos >= 0)
		{
			Result.y = atof(str.Left(nPos).GetString());
			str = str.Right(str.GetLength() - nPos - 1);
			nPos = str.Find(',');
			if (nPos >= 0)
			{
				Result.z = atof(str.Left(nPos).GetString());
				Result.w = atof(str.Right(str.GetLength() - nPos - 1).GetString());
			}
		}
	}
	return Result;
}

//从一个文件绝对路径取文件路径
CString GetFilePathFromFileName(CString FileName)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR * 2];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath_s(FileName.GetString(), drive, _MAX_DRIVE, dir, _MAX_DIR * 2, fname, _MAX_FNAME, ext, _MAX_EXT);

	CString path;
	path.Format("%s%s", drive, dir);
	return path;
}
CMain3DWnd::CMain3DWnd()
{
	m_isInitOgre = false;

	m_isExit = false;

	m_CurSTLFilePath = "D:\\CT_3D_Program\\bin\\肌肉.stl";
	m_pShareMemoryMgr = NULL;

	m_STLModel = NULL;

	m_ChangeShapeCenter = Ogre::Vector4::ZERO;
	m_ChangeShapePower = Ogre::Vector4(1, 1, 100, 0);

	m_XmlInterface = NULL;

	HINSTANCE CurModule =  LoadLibrary("XmlParse.dll");
	if (CurModule != NULL)
	{
		typedef IXMLParse* (*XML_CreateParse)();
		XML_CreateParse CurXmlInterface = (XML_CreateParse)GetProcAddress(CurModule, "XML_CreateParse");
		if (CurXmlInterface != NULL)
		{
			m_XmlInterface = CurXmlInterface();
		}
	}
}


CMain3DWnd::~CMain3DWnd()
{
	root->queueEndRendering();
	root->shutdown();
	delete root;
	root = NULL;
	m_isExit = true;
}

bool CMain3DWnd::Init()
{
	m_pShareMemoryMgr = new CommShareMemoryMgr();
	if (m_pShareMemoryMgr)
	{
		m_pShareMemoryMgr->InitShareMemoryMgr(false, "Test_Memory");
		m_pShareMemoryMgr->WriteShareMemory("TestInfdo", strlen("TestInfdo"));
		m_pShareMemoryMgr->SetShareMemoryNotify(this);
		m_pShareMemoryMgr->OnRun();
	}

	InitOgre(this->GetSafeHwnd());

	
	SetTimer((UINT)GetSafeHwnd(), 5, NULL);
	return true;
}

void CMain3DWnd::SetCmdMsgCallback(CmdMsgCallback pCmdMsgCallback)
{
	m_GuiCallBack = pCmdMsgCallback;
}

int CMain3DWnd::SetEnginWndPos(int x, int y, int w, int h)
{
	CRect rc(x, y, w, h);
	MoveWindow(&rc);
	if (NULL != MainRenderWnd)
	{
		MainRenderWnd->resize(w, h);
	}
	if (NULL != MainCamera)
	{
		MainCamera->setAspectRatio((Ogre::Real)MainViewPort->getActualWidth() / (Ogre::Real)MainViewPort->getActualHeight());
	}
	return 0;
}

void CMain3DWnd::SendCmdMsg(char * szMsg)
{
	std::string strObj, strCode;
	CString strParam;
	char *Del = " ";
	char buff[1024];
	char* Next_token = NULL;
	strcpy_s(buff, szMsg);

	strObj = strtok_s(buff, Del, &Next_token);  //分解字符串为一组标记串
	strCode = strtok_s(NULL, Del, &Next_token);
	strParam.Format("%s", Next_token);

	if (strObj == "LOAD")
	{
		if (strCode == "STLS")
		{
			if (0 != m_CurSTLFilePath.Compare(strParam))
			{
				if (m_STLModel)
				{
					MainNode->removeAllChildren();
					delete m_STLModel;
					m_STLModel = NULL;
				}
				m_CurSTLFilePath = strParam;
			}
			if (NULL == m_STLModel)
			{
				LoadSTLS();
			}
		}
	}
	else if(strObj == "MSG")
	{
		float CurValue = atof(strParam.GetString());
		if (strCode == "PRESSURE")
		{
			m_ChangeShapePower.z = CurValue;
		}
		else if(strCode == "AREA")
		{
			m_ChangeShapePower.w = CurValue;
		}
		UpdateChape();
	}
}

void CMain3DWnd::NotifyShareMemory(const char *szText, long lSize)
{
	char* tmp = new char[lSize];
	//OutputDebugString(szText);
	//OutputDebugString("\n");
	CString CurData;
	strcpy(tmp, szText);
	CurData.Format("%s", tmp);
	Ogre::Vector3 nData = ConvertCStringToVector3(CurData);
	m_ChangeShapePower = Ogre::Vector4(nData);
	m_ChangeShapePower.z = fabs(m_ChangeShapePower.z);

	//float4中最后一位w用来设置范围的基数
	m_ChangeShapePower.w = 36;
	UpdateChape();
}

void CMain3DWnd::InitOgre(HWND hWnd)
{
	CString PluginsName = "plugins.cfg";
#ifdef _DEBUG
	PluginsName = "plugins_d.cfg";
#endif // _DEBUG


	root = new Ogre::Root(PluginsName.GetString(), "ogre.cfg", "ogre.log");

	const Ogre::RenderSystemList rList = root->getAvailableRenderers();
	Ogre::RenderSystem* FirstRenderSystem = *(rList.begin());
	root->setRenderSystem(FirstRenderSystem);
	//if (!root->showConfigDialog())
	//{
	// 	return;
	//}

	root->initialise(false);

	m_isInitOgre = true;

	Ogre::NameValuePairList opts;

	opts["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned int)hWnd);


	MainRenderWnd = root->createRenderWindow("Ogre", 800, 600, true, &opts);

	InitRes();

	Ogre::MaterialManager::getSingleton().initialise();

	CurSceneMgr = root->createSceneManager(Ogre::ST_GENERIC);

	MainCamera = CurSceneMgr->createCamera("MainCam");
	MainCamera->setPosition(0, 200, 200);
	MainCamera->lookAt(0, 0, 0);

	MainViewPort = MainRenderWnd->addViewport(MainCamera);
	MainViewPort->setBackgroundColour(Ogre::ColourValue::Black);
	CurSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1, 1));

	MainCamera->setAspectRatio((Ogre::Real)MainViewPort->getActualWidth() / (Ogre::Real)MainViewPort->getActualHeight());
	MainCamera->setNearClipDistance(5);
	MainCamera->setFixedYawAxis(true);
	MainCamera->setAutoTracking(false);
	MainLight = CurSceneMgr->createLight("MainLight");
	MainLight->setType(Ogre::Light::LT_POINT);
	//MainLight->setSpotlightOuterAngle(Ogre::Radian(Ogre::Degree(100.0)));
	MainLight->setSpecularColour(1, 1, 1);
	MainLight->setDiffuseColour(1, 1, 1);
	MainLight->setSpotlightRange(Ogre::Degree(30.0), Ogre::Degree(50.0));
	MainLight->setCastShadows(true);
	//Ogre::SceneNode* LightNode = CurSceneMgr->getRootSceneNode()->createChildSceneNode();
	//LightNode->attachObject(MainLight);
	//LightNode->setPosition(100, 100, 100);

	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 100000, 100000, 10, 10, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);

	// create a floor entity, give it a material, and place it at the origin
	Ogre::Entity* floor = CurSceneMgr->createEntity("Floor", "floor");
	floor->setMaterialName("CT/GridFloor");
	floor->setCastShadows(false);
	floor->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScale(0.05, 0.05);
	CurSceneMgr->getRootSceneNode()->attachObject(floor);
	floor->getSubEntity(0)->getMaterial()->setReceiveShadows(true);

	CurSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 50);

	//InitTerr();

	MainNode = CurSceneMgr->getRootSceneNode()->createChildSceneNode();
	//root->startRendering();

	m_LAndRButtonDownState.first = false;
	m_LAndRButtonDownState.second = false;
	m_ButtonDownPos.x = 0;
	m_ButtonDownPos.y = 0;
}

void CMain3DWnd::InitRes()
{
	CString ResourceName = "resources.cfg";
#ifdef _DEBUG
	ResourceName = "resources_d.cfg";
#endif // _DEBUG


	Ogre::ConfigFile cf;//ConfigFile类：简单加载和解析配置文件
	cf.load(ResourceName.GetString());//导入资源文件
							   //首先获取一个可以遍历配置文件的每个区块的迭代器
	Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();//段
																			//定义三个字符串以保存从配置文件中提取的存数据并且遍历每个区块
	Ogre::String sectionName, typeName, dataname;
	while (sectionIter.hasMoreElements())
	{
		//获取区块的名称
		sectionName = sectionIter.peekNextKey();//获得资源管理器中的下一个的key（map映射的关系）
												//获取区块中包含的设置，同时，提前创建一个区块的迭代器
		Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;//声明多重映射的对象
													   //在区块中遍历每个设置
		for (i = settings->begin(); i != settings->end(); ++i)//使控制容器中的索引都对应着材质的数据
		{
			//使用迭代器来获取资源的名称和类型
			typeName = i->first;
			dataname = i->second;
			//使用资源名称，类型和区块的名称并添加至资源索引
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataname, typeName, sectionName);//资源定位
		}
	}

	Ogre::String sec, type, arch;

	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList("General");
	arch = genLocs.front()->archive->getName();
	arch = Ogre::StringUtil::replaceAll(arch, "Media/../../Tests/Media", "");
	arch = Ogre::StringUtil::replaceAll(arch, "media/../../Tests/Media", "");
	type = "FileSystem";
	sec = "Popular";

#ifdef OGRE_BUILD_PLUGIN_CG
	bool use_HLSL_Cg_shared = true;
#else
	bool use_HLSL_Cg_shared = Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl");
#endif

	// Add locations for supported shader languages
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL150", type, sec);
		}
		else
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL", type, sec);
		}

		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL400", type, sec);
		}
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/HLSL", type, sec);
	}
#ifdef OGRE_BUILD_PLUGIN_CG
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/Cg", type, sec);
#endif
	if (use_HLSL_Cg_shared)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/HLSL_Cg", type, sec);
	}

#ifdef INCLUDE_RTSHADER_SYSTEM
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/RTShaderLib/GLSLES", type, sec);
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/RTShaderLib/GLSL", type, sec);
		if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/RTShaderLib/GLSL150", type, sec);
		}
	}
	else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/RTShaderLib/HLSL", type, sec);
	}
#ifdef OGRE_BUILD_PLUGIN_CG
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/RTShaderLib/Cg", type, sec);
#endif
	if (use_HLSL_Cg_shared)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/RTShaderLib/HLSL_Cg", type, sec);
	}
#endif /* INCLUDE_RTSHADER_SYSTEM */


	//如果现在不想要索引更多的资源，可以仅索引添加的资源
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();//初始化，后续才可以加载mesh文件
}

void CMain3DWnd::InitTerr()
{
	//首先需要创建terrain options
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	Ogre::TerrainGroup* mTerrainGroup;
	mTerrainGlobals = new Ogre::TerrainGlobalOptions();
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(3000);

	mTerrainGlobals->setLightMapDirection(MainLight->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(CurSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(MainLight->getDiffuseColour());



	//其次要创建TerrainGroup对象

	mTerrainGroup = new Ogre::TerrainGroup(CurSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
	mTerrainGroup->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);



	//然后设置Terrain Group
	// Configure default import settings for if we use imported image
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = 513;
	defaultimp.worldSize = 12000.0f;
	defaultimp.inputScale = 600;
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;
	// textures
	defaultimp.layerList.resize(3);
	defaultimp.layerList[0].worldSize = 100;
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	defaultimp.layerList[1].worldSize = 30;
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	defaultimp.layerList[2].worldSize = 200;
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");


	//最后执行加载
	mTerrainGroup->loadAllTerrains(true);
}

void CMain3DWnd::LoadSTLS()
{
	if (m_XmlInterface == NULL)
	{
		return;
	}

	if (!m_XmlInterface->Init(m_CurSTLFilePath.GetString()))
	{
		return;
	}

	IXMLNode* RootNode =  m_XmlInterface->GetDocumentNode();
	if (RootNode != NULL)
	{
		double MaxRadius = 0;
		IXMLNode* StlNode = RootNode->GetFirstChildNode();
		//取出变形点
		m_ChangeShapeCenter = Ogre::Vector4(ConvertCStringToVector3(StlNode->GetAttribute("ChangeShapePos")));
		StlNode = StlNode->GetChildrenNode("Node");
		while (StlNode != NULL)
		{
			//名称
			CString CurName =  StlNode->GetAttribute("Name");
			//Stl路径
			CString CurStlPath = StlNode->GetAttribute("ResPath");
			CString CurModelPath = GetFilePathFromFileName(m_CurSTLFilePath) + "\\" + CurStlPath;
			//位置
			CString CurStlPos = StlNode->GetAttribute("Pos");
			Ogre::Vector3 CurPos = ConvertCStringToVector3(CurStlPos);
			//姿态
			CString CurStlPose = StlNode->GetAttribute("Orientation");
			Ogre::Vector4 CurPose = ConvertCStringToVector4(CurStlPose);
			Ogre::Quaternion CurQua = Ogre::Quaternion(CurPose.w, CurPose.x, CurPose.y, CurPose.z);

			STLInfo CurInfo;

			CurInfo.pStlInterface = new CDrawSTLMesh(CurModelPath, CurSceneMgr);

			CurInfo.pStlNode = MainNode->createChildSceneNode(CurName.GetString(), CurPos, CurQua);

			Ogre::ManualObject* CurStlMode = CurInfo.pStlInterface->GetStlModePtr();

			if (CurInfo.pStlNode != NULL && CurStlMode != NULL)
			{
				CurInfo.pStlNode->attachObject(CurStlMode);
				//CurInfo.pStlNode->roll(Ogre::Degree(180.0));
				Ogre::Quaternion Curq = CurInfo.pStlNode->getOrientation();
				double CurRadius = CurStlMode->getBoundingRadius();
				if (CurRadius > MaxRadius)
				{
					MaxRadius = CurRadius;
				}
				//m_ChangeShapeCenter.y = -CurRadius / 2;
				//Ogre::GpuProgramParametersSharedPtr fparams =
				//	CurStlMode->getSection(0)->getTechnique()->getPass(0)->getVertexProgramParameters();
				//fparams->setNamedConstant("CenterPos", m_ChangeShapeCenter);
				CurStlMode->setCastShadows(true);
			}
			m_StlInfoList[CurName] = CurInfo;
			StlNode = StlNode->GetNextSiblingNode();
		}
		double nLength = MaxRadius * 1.5;
		Ogre::Vector3 ModePos(0, nLength, 0);
		MainNode->setPosition(ModePos);
		MainCamera->setPosition(nLength * 2, nLength * 2, nLength * 2);
		MainCamera->lookAt(ModePos);
		//MainCamera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
		ModePos.y += MaxRadius;
		MainLight->setPosition(ModePos);
		//ModePos = -ModePos;
		ModePos.normalise();
		MainLight->setDirection(ModePos);
		UpdateChape();
	}

}

void CMain3DWnd::UpdateChape()
{
	for (STLInfoMap::iterator it = m_StlInfoList.begin(); it != m_StlInfoList.end(); ++it)
	{
		STLInfo CurInfo = it->second;
		//CurInfo.pStlInterface->GetStlModePtr()->getSection(0)->setCustomParameter(13, m_ChangeShapePower);
		//Ogre::GpuProgramParametersSharedPtr fparams =
		//	CurInfo.pStlInterface->GetStlModePtr()->getSection(0)->getTechnique()->getPass(0)->getVertexProgramParameters();
		//fparams->setNamedConstant("ForcePower", m_ChangeShapePower);
		CurInfo.pStlInterface->DeformMesh(Ogre::Vector3(m_ChangeShapeCenter.x, m_ChangeShapeCenter.y, m_ChangeShapeCenter.z), m_ChangeShapePower.z, m_ChangeShapePower.w);
	}
}


BOOL CMain3DWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::PreTranslateMessage(pMsg);
}


BOOL CMain3DWnd::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::DestroyWindow();
}


BOOL CMain3DWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_KEYDOWN)
	{
		OutputDebugString("KeyDown\n");
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}
BEGIN_MESSAGE_MAP(CMain3DWnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()


void CMain3DWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_LAndRButtonDownState.first = true;
	m_ButtonDownPos.x = point.x;
	m_ButtonDownPos.y = point.y;
	SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}


void CMain3DWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LAndRButtonDownState.first = false;

	CWnd::OnLButtonUp(nFlags, point);
}


void CMain3DWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	int xDis = point.x - m_ButtonDownPos.x;
	int yDis = point.y - m_ButtonDownPos.y;
	xDis *= -1;
	yDis *= -1;
	Ogre::Vector3 OldCamPos = MainCamera->getPosition();
	Ogre::Vector3 MainNodePos = MainNode->getPosition();
	Ogre::Vector3 MainToCam = -(MainNodePos - OldCamPos);
	float nLength = MainToCam.length();
	Ogre::Vector3 OldDir = MainToCam.normalisedCopy();
	Ogre::Vector3 MainNodeLeft = OldDir.crossProduct(Ogre::Vector3::UNIT_Y);
	if (m_LAndRButtonDownState.first)
	{
		//MainCamera->moveRelative(Ogre::Vector3(xDis * 0.03, yDis * 0.005, 0));
		//TRACE("xDis=%d, yDis=%d\n", xDis, yDis);
		Ogre::Quaternion QYaw, QPitch;
		QYaw.FromAngleAxis(Ogre::Radian(Ogre::Degree(xDis)), Ogre::Vector3::NEGATIVE_UNIT_Y);
		QPitch.FromAngleAxis(Ogre::Radian(Ogre::Degree(yDis)), MainNodeLeft);
		Ogre::Vector3 NewDir = QYaw * QPitch * OldDir;
		NewDir.normalise();
		Ogre::Vector3 NewPos = MainNodePos + NewDir * nLength;
		MainCamera->setPosition(NewPos);
		//MainCamera->setDirection(OldDir);
		MainCamera->lookAt(MainNodePos);
	}
	else if (m_LAndRButtonDownState.second)
	{
		MainCamera->pitch(Ogre::Degree(yDis * 0.05f));
		MainCamera->yaw(Ogre::Degree(xDis * 0.05f));
	}

	if (m_MbuttonDownState)
	{
		OldCamPos += MainCamera->getRight() * xDis + MainCamera->getUp() * yDis;
		MainCamera->setPosition(OldCamPos);
	}
	m_ButtonDownPos.x = point.x;
	m_ButtonDownPos.y = point.y;

	CWnd::OnMouseMove(nFlags, point);
}


BOOL CMain3DWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	MainCamera->moveRelative(Ogre::Vector3(0, 0, -zDelta * 0.1));

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CMain3DWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_LAndRButtonDownState.second = true;
	m_ButtonDownPos.x = point.x;
	m_ButtonDownPos.y = point.y;
	
	CWnd::OnRButtonDown(nFlags, point);
}


void CMain3DWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_LAndRButtonDownState.second = false;

	CWnd::OnRButtonUp(nFlags, point);
}


void CMain3DWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (m_isInitOgre)
	{
		root->renderOneFrame();
	}

	CWnd::OnTimer(nIDEvent);
}


void CMain3DWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP)
	{
		m_ChangeShapePower.z -= 0.1;
	}
	else if(nChar == VK_DOWN)
	{
		m_ChangeShapePower.z += 0.1;
	}
	else if (nChar == 'A')
	{
		Ogre::Vector3 OldCamPos = MainCamera->getPosition();
		OldCamPos -= MainCamera->getRight() * 2.0;
		MainCamera->setPosition(OldCamPos);
	}
	else if(nChar == 'D')
	{
		Ogre::Vector3 OldCamPos = MainCamera->getPosition();
		OldCamPos += MainCamera->getRight() * 2.0 ;
		MainCamera->setPosition(OldCamPos);
	}
	else if(nChar == 'W')
	{
		Ogre::Vector3 OldCamPos = MainCamera->getPosition();
		OldCamPos += MainCamera->getUp() * 2.0;
		MainCamera->setPosition(OldCamPos);
	}
	else if(nChar == 'S')
	{
		Ogre::Vector3 OldCamPos = MainCamera->getPosition();
		OldCamPos -= MainCamera->getUp() * 2.0;
		MainCamera->setPosition(OldCamPos);
	}

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMain3DWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_MbuttonDownState = true;

	__super::OnMButtonDown(nFlags, point);
}


void CMain3DWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_MbuttonDownState = false;

	__super::OnMButtonUp(nFlags, point);
}
