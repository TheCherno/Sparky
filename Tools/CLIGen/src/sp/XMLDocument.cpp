#include "sp.h"
#include "XMLDocument.h"

#include "Util.h"

namespace sp {

	using namespace rapidxml;

	const XMLNode* XMLNode::NullNode = new XMLNode("NULL", *(XMLNode*)NULL);

	XMLDocument::XMLDocument(const String& name, const String& filepath)
		: m_Name(name), m_Filepath(filepath)
	{
		char* file = ReadFile(filepath);

		xml_document<> doc;
		doc.parse<0>(file);

		xml_node<>* root = doc.first_node();
		m_RootNode = new XMLNode(root->name());

		ParseNodeChildren(root, *m_RootNode);
		ParseAllAttributes(root, m_RootNode->attributes);
	}

	XMLDocument::~XMLDocument()
	{
		delete m_RootNode;
	}

	XMLNode* XMLDocument::FindNode(const String& name)
	{
		if (m_RootNode->name == name)
			return m_RootNode;

		return FindChild(*m_RootNode, name);
	}

	void XMLDocument::ParseAllAttributes(xml_node<>* node, std::vector<XMLAttribute>& attributes)
	{
		for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute())
			attributes.push_back({ String(attribute->name()), String(attribute->value()) });
	}

	void XMLDocument::ParseNodeChildren(rapidxml::xml_node<>* node, XMLNode& base)
	{
		for (xml_node<>* child = node->first_node(); child; child = child->next_sibling())
		{
			base.children.push_back(XMLNode(child->name(), child->value(), base));
			ParseAllAttributes(child, base.children.back().attributes);
			ParseNodeChildren(child, base.children.back());
		}
	}

	XMLNode* XMLDocument::FindChild(XMLNode& node, const String& name)
	{
		for (XMLNode& child : node.children)
		{
			if (child.name == name)
				return &child;

			return FindChild(child, name);
		}
		return nullptr;
	}

}