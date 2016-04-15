#pragma once

#include <sp/SPTypes.h>
#include "rapidxml.hpp"
#include "cligen/String.h"

namespace sp {

	struct XMLAttribute
	{
		String name;
		String value;
	};

	struct XMLNode
	{
		static const XMLNode* NullNode;

		String name;
		String value;
		std::vector<XMLAttribute> attributes;
		std::vector<XMLNode> children;
		const XMLNode& parent;

		XMLNode(const String& name, const XMLNode& parent = *NullNode)
			: name(name), parent(parent) {}
		XMLNode(const String& name, const String& value, const XMLNode& parent = *NullNode)
			: name(name), value(value), parent(parent) {}

		XMLAttribute* FindAttribute(const String& name)
		{
			for (XMLAttribute& attribute : attributes)
			{
				if (attribute.name == name)
					return &attribute;
			}
			return nullptr;
		}
	};

	class XMLDocument
	{
	private:
		String m_Name, m_Filepath;
		XMLNode* m_RootNode;
	public:
		XMLDocument(const String& name, const String& filepath);
		~XMLDocument();

		inline XMLNode& GetRootNode() const { return *m_RootNode; }
		XMLNode* FindNode(const String& name);
	private:
		void ParseAllAttributes(rapidxml::xml_node<>* node, std::vector<XMLAttribute>& attributes);
		void ParseNodeChildren(rapidxml::xml_node<>* node, XMLNode& base);

		XMLNode* FindChild(XMLNode& node, const String& name);
	};

}