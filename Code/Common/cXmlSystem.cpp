#include "cXmlSystem.h"
#include <fstream>

using namespace std;

namespace cross
{

//static cpstr c_sXmlCac = "xmlcac";

iXmlReader* iXmlReader::CreateInstance()
{
	return new cXmlReader();
}

cXmlReader::cXmlReader()
{
	m_pHandler = 0;
}

cXmlReader::~cXmlReader()
{
	Unload();
}

void cXmlReader::Release()
{
	delete this;
}

bool cXmlReader::Load(cpstr file, iXmlReaderHandler& handle)
{
	std::fstream f(file, std::ios_base::in | std::ios_base::binary);
	if (!f.is_open())
		return false;
	f.seekg(0, std::ios_base::end);
	std::streamsize len = f.tellg();
	f.seekg(0, std::ios_base::beg);
	if (len == 0)
		return false;
	bool bOK = false;
	i8* buf = new i8[(ui32)len];
	if (f.read(buf, len).gcount() != len)
		goto err;
	bOK = Load(buf, handle);
err:
	delete[] buf;
	return bOK;
}

bool cXmlReader::LoadString(cpstr buf, iXmlReaderHandler& handle)
{
	m_pHandler = &handle;
	TiXmlDocument doc;
	doc.Parse(buf);
	const TiXmlElement* currElement = doc.RootElement();
	if (currElement) {
		ProcessElement(currElement);
		return true;
	}
	else
		return false;
}

void cXmlReader::Unload()
{
}

void cXmlReader::ProcessElement(const TiXmlElement* element)
{
	cXmlAttributes attrs;
	const TiXmlAttribute *currAttr = element->FirstAttribute();
	while (currAttr) {
		attrs.Add(currAttr->Name(), currAttr->Value());
		currAttr = currAttr->Next();
	}
	m_pHandler->ElementStart(element->Value(), attrs, element->GetText());
	const TiXmlElement* childElement = element->FirstChildElement();
	while (childElement) {
		ProcessElement(childElement);
		childElement = childElement->NextSiblingElement();
	}
	m_pHandler->ElementEnd(element->Value());
}

class cTestXmlHandler : public iXmlReaderHandler
{
public:

	void ElementStart(cpstr element, const iXmlAttributes& attributes) {
		LInfo << "Start " << element << LEnd;
	}

	void ElementEnd(cpstr element) {
		LInfo << "End " << element << LEnd;
	}
};

void iXmlReader::test()
{
	iXmlReader* p = iXmlReader::CreateInstance();
	cTestXmlHandler th;
	p->Load("MainWindow.xml", th);
	p->Release();
}

}
