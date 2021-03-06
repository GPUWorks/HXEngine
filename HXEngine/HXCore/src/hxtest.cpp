#include "hxtest.h"
#include "tinyxml.h"
#include <iostream>


hxtest::hxtest()
{
}


hxtest::~hxtest()
{
}

bool hxtest::LoadFile(std::string strFileName)
{
	TiXmlDocument doc(strFileName.c_str());
	bool bLoaded = doc.LoadFile();
	doc.Print();
	if (!bLoaded)
	{
		return false;
	}

	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* animElement = rootElement->FirstChildElement();
	for (; animElement != NULL; animElement = animElement->NextSiblingElement())
	{
		/*TiXmlAttribute* attributeOfAnim = animElement->FirstAttribute();
		for (; attributeOfAnim != NULL; attributeOfAnim = attributeOfAnim->Next())
		{
			std::cout << attributeOfAnim->Name() << " : " << attributeOfAnim->Value() << std::endl;
		}*/
		std::string strName = animElement->Attribute("name");
		std::string strFileName = animElement->Attribute("File");

		AnimInfo animInfo;
		animInfo.strAnimName = strName;
		animInfo.strFileName = strFileName;
		vctAnimsInfo.push_back(animInfo);
	}

	return true;
}

//int main()
//{
//	hxtest* hx = new hxtest;
//	hx->LoadFile("DogAnim.xml");
//	return 0;
//}