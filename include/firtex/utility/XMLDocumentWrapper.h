//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-12-27 22:40:41

#ifndef __FX_XMLDOCUMENTWRAPPER_H
#define __FX_XMLDOCUMENTWRAPPER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/rapidxml.hpp"
#include "firtex/utility/rapidxml_print.hpp"
#include <string>

FX_NS_DEF(utility);

class XMLNodeWrapper;
DEFINE_TYPED_PTR(XMLNodeWrapper);

class XMLNodeBase
{
public:
    typedef rapidxml::xml_base<char> XMLBase;
    typedef rapidxml::xml_node<char> XMLNode;
    typedef rapidxml::xml_attribute<char> XMLAttribute;
    
public:
    XMLNodeBase();
    XMLNodeBase(XMLBase* pNode);
    virtual ~XMLNodeBase() {}

public:
    /**
     * Return node name
     */
    const char* getName() const;

    /**
     * Return node value
     */
    const char* getValue() const;

    /**
     * Return parent node
     */
    XMLNodeWrapperPtr parent() const;

public:
    XMLBase* getNodeBase() const;

protected:
    XMLBase* m_pNodeBase;
};

class XMLAttributeWrapper;
DEFINE_TYPED_PTR(XMLAttributeWrapper);

class XMLAttributeWrapper : public XMLNodeBase
{
public:
    typedef rapidxml::xml_attribute<char> XMLAttribute;

public:
    XMLAttributeWrapper(XMLAttribute* pAttribute);

public:
    /**
     * Return previous attribute, optionally matching attribute name. 
     * @param sName attribute name
     */
    XMLAttributeWrapperPtr previousAttribute(const std::string& sName = "");

    /**
     * Return next attribute, optionally matching attribute name. 
     * @param sName attribute name
     */
    XMLAttributeWrapperPtr nextAttribute(const std::string& sName = "");

public:
    XMLAttribute* getAttribute() const {return m_pAttribute;}

private:
    XMLAttribute* m_pAttribute;
};

class XMLNodeWrapper : public XMLNodeBase
{
public:
//    typedef rapidxml::node_type NodeType;
    enum NodeType
    {
        NODE_DOCUMENT,      // A document node. Name and value are empty.
        NODE_ELEMENT,       // An element node. Name contains element name. Value contains text of first data node.
        NODE_DATA,          // A data node. Name is empty. Value contains data text.
        NODE_CDATA,         // A CDATA node. Name is empty. Value contains data text.
        NODE_COMMENT,       // A comment node. Name is empty. Value contains comment text.
        NODE_DECLARATION,   // declaration node. Name and value are empty.
                            // Declaration parameters (version, encoding and standalone) are in node attributes.
        NODE_DOCTYPE,       // A DOCTYPE node. Name is empty. Value contains DOCTYPE text.
        NODE_PI             // A PI node. Name contains target. Value contains instructions.
    };

    typedef rapidxml::xml_document<char> XMLDocument;

public:
    XMLNodeWrapper();
    XMLNodeWrapper(XMLNode* pXmlNode);
    ~XMLNodeWrapper() {}

public:
    /**
     * Return node type
     */
    NodeType type() const;

    /**
     * Return first child node, optionally matching node name
     * @param sName node name
     */
    XMLNodeWrapperPtr firstNode(const std::string& sName = "") const;

    /**
     * Return last child node, optionally matching node name
     * @param sName node name
     */
    XMLNodeWrapperPtr lastNode(const std::string& sName = "") const;

    /**
     * Return previous sibling node, optionally matching node name
     * @param sName node name
     */
    XMLNodeWrapperPtr previousSibling(const std::string& sName = "") const;

    /**
     * Return next sibling node, optionally matching node name
     * @param sName node name
     */
    XMLNodeWrapperPtr nextSibling(const std::string& sName = "") const;

    /**
     * Return first attribute node, optionally matching attribute name
     * @param sName attribute name
     */
    XMLAttributeWrapperPtr firstAttribute(const std::string& sName = "") const;
    
    /**
     * Return last attribute node, optionally matching attribute name
     * @param sName attribute name
     */
    XMLAttributeWrapperPtr lastAttribute(const std::string& sName = "") const;

public:
    /**
     * Prepends a new child node.
     * The prepended child becomes the first child,
     * and all existing children are moved one position back.
     * @param child Node to prepend.
     */
    void prependNode(const XMLNodeWrapperPtr& pChild);

    /**
     * Appends a new child node. 
     * The appended child becomes the last child.
     * @param child Node to append.
     */
    void appendNode(const XMLNodeWrapperPtr& pChild);

    /**
     * Prepends a new child node.
     * The prepended child becomes the first child,
     * and all existing children are moved one position back.
     * @param type node type
     * @param sName node name
     * @param sValue node value
     * @return the new child node
     */
    XMLNodeWrapperPtr prependNode(NodeType type, const std::string& sName, 
                                  const std::string& sValue = "");

    /**
     * Appends a new child node. 
     * The appended child becomes the last child.
     * @param type node type
     * @param sName node name
     * @param sValue node value
     * @return the new child node
     */
    XMLNodeWrapperPtr appendNode(NodeType type, const std::string& sName = "", 
                                 const std::string& sValue = "");

    /**
     * Prepends a new attribute to the node.
     * @param attribute Attribute to prepend.
     */
    void prependAttribute(const XMLAttributeWrapperPtr& pAttribute);

    /**
     * Appends a new attribute to the node.
     * @param attribute Attribute to append.
     */
    void appendAttribute(const XMLAttributeWrapperPtr& pAttribute);

    /**
     * Prepends a new attribute to the node.
     * @param sName attribute name
     * @param sValue attribute value
     * @return the new attribute node
     */
    XMLAttributeWrapperPtr prependAttribute(const std::string& sName,
            const std::string& sValue);

    /**
     * Appends a new attribute to the node.
     * @param attribute Attribute to append.
     * @param sName attribute name
     * @param sValue attribute value
     * @return the new attribute node
     */
    XMLAttributeWrapperPtr appendAttribute(const std::string& sName,
            const std::string& sValue);

    XMLNode* getNode() const;

protected:
    XMLNode* m_pNode;

protected:
    XMLDocument* document() const;
};

class XMLDocumentWrapper : public XMLNodeWrapper
{
public:
    typedef rapidxml::xml_node<char> XMLNode;
    typedef rapidxml::xml_attribute<char> XMLAttribute;

public:
    XMLDocumentWrapper();
    ~XMLDocumentWrapper();

public:
    /**
     * Parse xml string
     * @param sXML xml string
     * @throw throw BadXmlFormatException
     */
    void parse(const std::string& sXML);

    /**
     * Parse xml file
     * @param sFile file path
     * @throw throw BadXmlFormatException
     */
    void parseFile(const std::string& sFile);

    /**
     * Clear the document by deleting all nodes
     */
    void clear();

    /**
     * Print xml to given output stream
     * @param out Output stream to print to.
     */
    void print(std::basic_ostream<char>& out);

    /**
     * Print xml to given string
     * @param out Output string to print to.
     */
    void print(std::string& out);

    /**
     * Print xml to given string
     * @param out Output string to print to.
     */
    void printToFile(std::string& sFile);

public:
    /**
     * Add declaration node
     * @param sEncoding encoding attribute of declaration node
     */
    XMLNodeWrapperPtr addDeclarationNode(const std::string& sEncoding);

    /**
     * Return declaration child node
     * @param sName node name
     */
    XMLNodeWrapperPtr declarationNode() const;

    /**
     * Allocate a new node from the pool, and optionally assigns name and value to it. 
     * @param type Type of node to create.
     * @param sName Name to assign to the node, or 0 to assign no name.
     * @param sValue Value to assign to the node, or 0 to assign no value.
     * @return Pointer to allocated node. This pointer will never be NULL.
     * @throw If the allocation request cannot be accomodated,
     *        this function will throw <code>OutOfMemoryException</code>.
     */
    XMLNodeWrapperPtr allocateNode(NodeType type, const std::string& sName, 
                                   const std::string& sValue = "");

    /**
     * Allocate a new attribute from the pool, and optionally assigns name and value to it.
     * @param sName Name to assign to the attribute, or 0 to assign no name.
     * @param sValue Value to assign to the attribute, or 0 to assign no value.
     * @return Pointer to allocated attribute. This pointer will never be NULL.
     * @throw If the allocation request cannot be accomodated,
     *        this function will throw <code>OutOfMemoryException</code>.
     */
    XMLAttributeWrapperPtr allocateAttribute(const std::string& sName = "",
            const std::string& sValue = "");

    /**
     * Clones an xml_node and its hierarchy of child nodes and attributes.
     * Nodes and attributes are allocated from this memory pool.
     * Names and values are not cloned, they are shared between the clone and the source.
     * Result node can be optionally specified as a second parameter, 
     * in which case its contents will be replaced with cloned source node.
     * This is useful when you want to clone entire document.
     *
     * @param source Node to clone.
     * @return Pointer to cloned node. This pointer will never be NULL.
     */
    XMLNodeWrapperPtr cloneNode(const XMLNodeWrapperPtr& src);

private:
    XMLDocument m_doc;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(XMLDocumentWrapper);

///////////////////////////////////////////////////////
//XMLNodeBase
inline XMLNodeBase::XMLNodeBase()
    : m_pNodeBase(NULL)
{
}

inline XMLNodeBase::XMLNodeBase(XMLBase* pNode)
    : m_pNodeBase(pNode)
{
}

inline const char* XMLNodeBase::getName() const
{
    return m_pNodeBase->name();
}

inline const char* XMLNodeBase::getValue() const
{
    return m_pNodeBase->value();
}

inline XMLNodeWrapperPtr XMLNodeBase::parent() const
{
    XMLNode* pNode = m_pNodeBase->parent();
    if (pNode)
    {
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    return XMLNodeWrapperPtr();
}

inline XMLNodeBase::XMLBase* XMLNodeBase::getNodeBase() const
{
    return m_pNodeBase;
}


/////////////////////////////////////////////////////////
//XMLNodeWrapper
inline XMLNodeWrapper::XMLNodeWrapper()
{
}

inline XMLNodeWrapper::XMLNodeWrapper(XMLNode* pXmlNode)
    : XMLNodeBase(pXmlNode)
    , m_pNode(pXmlNode)
{
}

inline XMLNodeWrapper::NodeType XMLNodeWrapper::type() const
{
    return (NodeType)m_pNode->type();
}

inline XMLNodeWrapperPtr
XMLNodeWrapper::firstNode(const std::string& sName) const
{
    XMLNode* pNode = m_pNode->first_node(
            sName.empty() ? NULL : sName.c_str());
    if (pNode && (NodeType)pNode->type() == NODE_DECLARATION)
    {
        pNode = pNode->next_sibling();
    }
    if (pNode)
    {
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    return XMLNodeWrapperPtr();
}

inline XMLNodeWrapperPtr
XMLNodeWrapper::lastNode(const std::string& sName) const
{
    XMLNode* pNode = m_pNode->last_node(
            sName.empty() ? NULL : sName.c_str());
    if (pNode)
    {
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    return XMLNodeWrapperPtr();
}

inline XMLNodeWrapperPtr
XMLNodeWrapper::previousSibling(const std::string& sName) const
{
    XMLNode* pNode = m_pNode->previous_sibling(
            sName.empty() ? NULL : sName.c_str());
    if (pNode)
    {
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    return XMLNodeWrapperPtr();
}

inline XMLNodeWrapperPtr 
XMLNodeWrapper::nextSibling(const std::string& sName) const
{
    XMLNode* pNode = m_pNode->next_sibling(
            sName.empty() ? NULL : sName.c_str());
    if (pNode)
    {
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    return XMLNodeWrapperPtr();
}

inline XMLAttributeWrapperPtr
XMLNodeWrapper::firstAttribute(const std::string& sName) const
{
    XMLAttribute* pNode = m_pNode->first_attribute(
            sName.empty() ? NULL : sName.c_str());
    if (pNode)
    {
        return XMLAttributeWrapperPtr(new XMLAttributeWrapper(pNode));
    }
    return XMLAttributeWrapperPtr();
}

inline XMLAttributeWrapperPtr
XMLNodeWrapper::lastAttribute(const std::string& sName) const
{
    XMLAttribute* pNode = m_pNode->last_attribute(
            sName.empty() ? NULL : sName.c_str());
    if (pNode)
    {
        return XMLAttributeWrapperPtr(new XMLAttributeWrapper(pNode));
    }
    return XMLAttributeWrapperPtr();
}

inline void XMLNodeWrapper::prependNode(const XMLNodeWrapperPtr& pChild)
{
    m_pNode->prepend_node(pChild->getNode());
}

inline void XMLNodeWrapper::appendNode(const XMLNodeWrapperPtr& pChild)
{
    m_pNode->append_node(pChild->getNode());
}

inline void XMLNodeWrapper::prependAttribute(const XMLAttributeWrapperPtr& pAttribute)
{
    m_pNode->prepend_attribute(pAttribute->getAttribute());
}

inline void XMLNodeWrapper::appendAttribute(const XMLAttributeWrapperPtr& pAttribute)
{
    m_pNode->append_attribute(pAttribute->getAttribute());
}

inline XMLNodeWrapper::XMLDocument* XMLNodeWrapper::document() const
{
    return m_pNode->document();
}

inline XMLNodeWrapperPtr
XMLNodeWrapper::prependNode(NodeType type, const std::string& sName, 
                            const std::string& sValue)
{
    XMLDocument* pDoc = document();
    if (pDoc)
    {
        XMLNode* pNode = pDoc->allocate_node(rapidxml::node_type(type),
                sName.empty() ? NULL : pDoc->allocate_string(sName.c_str()),
                sValue.empty() ? NULL : pDoc->allocate_string(sValue.c_str()));
        XMLNodeWrapperPtr pNodePtr(new XMLNodeWrapper(pNode));
        prependNode(pNodePtr);
        return pNodePtr;
    }
    return XMLNodeWrapperPtr();
}

inline XMLNodeWrapperPtr 
XMLNodeWrapper::appendNode(NodeType type,
                           const std::string& sName, 
                           const std::string& sValue)
{
    XMLDocument* pDoc = document();
    if (pDoc)
    {
        XMLNode* pNode = pDoc->allocate_node(rapidxml::node_type(type),
                sName.empty() ? NULL : pDoc->allocate_string(sName.c_str()),
                sValue.empty() ? NULL : pDoc->allocate_string(sValue.c_str()));
        XMLNodeWrapperPtr pNodePtr(new XMLNodeWrapper(pNode));
        appendNode(pNodePtr);
        return pNodePtr;
    }
    return XMLNodeWrapperPtr();
}

inline XMLAttributeWrapperPtr
XMLNodeWrapper::prependAttribute(const std::string& sName,
                                 const std::string& sValue)
{
    XMLDocument* pDoc = document();
    if (pDoc)
    {
        XMLAttribute* pAttr = pDoc->allocate_attribute(
                sName.empty() ? NULL : pDoc->allocate_string(sName.c_str()),
                sValue.empty() ? NULL : pDoc->allocate_string(sValue.c_str()));
        XMLAttributeWrapperPtr pAttrPtr(new XMLAttributeWrapper(pAttr));
        prependAttribute(pAttrPtr);
        return pAttrPtr;
    }
    return XMLAttributeWrapperPtr();
}

inline XMLAttributeWrapperPtr
XMLNodeWrapper::appendAttribute(const std::string& sName,
                                const std::string& sValue)
{
    XMLDocument* pDoc = document();
    if (pDoc)
    {
        XMLAttribute* pAttr = pDoc->allocate_attribute(
                sName.empty() ? NULL : pDoc->allocate_string(sName.c_str()),
                sValue.empty() ? NULL : pDoc->allocate_string(sValue.c_str()));
        XMLAttributeWrapperPtr pAttrPtr(new XMLAttributeWrapper(pAttr));
        appendAttribute(pAttrPtr);
        return pAttrPtr;
    }
    return XMLAttributeWrapperPtr();
}

inline XMLNodeWrapper::XMLNode* XMLNodeWrapper::getNode() const
{
    return m_pNode;
}

//////////////////////////////////////////////////////
//XMLAttributeWrapper
inline XMLAttributeWrapper::XMLAttributeWrapper(XMLAttribute* pAttribute)
    : XMLNodeBase(pAttribute)
    , m_pAttribute(pAttribute)
{
}

inline XMLAttributeWrapperPtr
XMLAttributeWrapper::previousAttribute(const std::string& sName)
{
    XMLAttribute* pAttr = m_pAttribute->previous_attribute(
            sName.empty() ? NULL : sName.c_str());
    if (pAttr)
    {
        return XMLAttributeWrapperPtr(new XMLAttributeWrapper(pAttr));
    }
    return XMLAttributeWrapperPtr();
}

inline XMLAttributeWrapperPtr
XMLAttributeWrapper::nextAttribute(const std::string& sName)
{
    XMLAttribute* pAttr = m_pAttribute->next_attribute(
            sName.empty() ? NULL : sName.c_str());
    if (pAttr)
    {
        return XMLAttributeWrapperPtr(new XMLAttributeWrapper(pAttr));
    }
    return XMLAttributeWrapperPtr();
}

/////////////////////////////////////////////////
//
inline XMLNodeWrapperPtr
XMLDocumentWrapper::addDeclarationNode(const std::string& sEncoding)
{
    XMLNode* pNode = m_pNode->first_node(NULL);
    if (pNode && (NodeType)pNode->type() == NODE_DECLARATION)
    {
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    else 
    {
        this->prependNode(XMLDocumentWrapper::NODE_DECLARATION, "");
        XMLNodeWrapperPtr pDeclNode = declarationNode();
        pDeclNode->appendAttribute(allocateAttribute("version", "1.0"));
        if (!sEncoding.empty())
        {
            pDeclNode->appendAttribute(allocateAttribute("encoding", sEncoding));
        }
        return pDeclNode;
    }
}

inline XMLNodeWrapperPtr XMLDocumentWrapper::declarationNode() const
{
    XMLNode* pNode = m_pNode->first_node(NULL);
    if (pNode && (NodeType)pNode->type() == NODE_DECLARATION)
    {
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    return XMLNodeWrapperPtr();
}

inline XMLNodeWrapperPtr XMLDocumentWrapper::allocateNode(
        NodeType type, const std::string& sName, 
        const std::string& sValue)
{
    try 
    {
        XMLNode* pNode = m_doc.allocate_node(rapidxml::node_type(type),
                sName.empty() ? NULL : m_doc.allocate_string(sName.c_str()),
                sValue.empty() ? NULL : m_doc.allocate_string(sValue.c_str()));
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));    
    }
    catch(std::bad_alloc& )
    {
        FIRTEX_THROW(OutOfMemoryException, "Allocate xml node FAILED.");
    }
    return XMLNodeWrapperPtr();
}

inline XMLAttributeWrapperPtr
XMLDocumentWrapper::allocateAttribute(const std::string& sName,
                                      const std::string& sValue)
{
    try 
    {
        XMLAttribute* pAttr = m_doc.allocate_attribute(
                sName.empty() ? NULL : m_doc.allocate_string(sName.c_str()),
                sValue.empty() ? NULL : m_doc.allocate_string(sValue.c_str()));
        return XMLAttributeWrapperPtr(new XMLAttributeWrapper(pAttr));
    }
    catch(std::bad_alloc& )
    {
        FIRTEX_THROW(OutOfMemoryException, "Allocate xml attribute FAILED.");
    }
    return XMLAttributeWrapperPtr();
}

inline XMLNodeWrapperPtr XMLDocumentWrapper::cloneNode(const XMLNodeWrapperPtr& src)
{
    try 
    {
        XMLNode* pNode = m_doc.clone_node(src->getNode(), NULL);
        return XMLNodeWrapperPtr(new XMLNodeWrapper(pNode));
    }
    catch(std::bad_alloc& )
    {
        FIRTEX_THROW(OutOfMemoryException, "Allocate xml attribute FAILED.");
    }
    return XMLNodeWrapperPtr();    
}

FX_NS_END

#endif //__FX_XMLDOCUMENTWRAPPER_H
