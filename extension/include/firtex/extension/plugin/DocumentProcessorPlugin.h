//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-09 00:07:01

#ifndef __FX_DOCUMENTPROCESSORPLUGIN_H
#define __FX_DOCUMENTPROCESSORPLUGIN_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/processor/DocumentProcessor.h"
#include "firtex/extension/com/ComPtr.h"

FX_NS_DEF(plugin);

class DocumentProcessorPlugin : public FX_NS(processor)::DocumentProcessor
{
public:
    class Creator : public FX_NS(processor)::DocumentProcessor::Creator
    {
    public: 
        Creator(IDocumentProcessorPlugin* pInterface)
            : m_pInterface(pInterface)
        {
            FX_NS(com)::FX_BSTR name;
            FX_NS(com)::FX_HRESULT ret = pInterface->getIdentifier(&name);
            FIRTEX_ASSERT2(ret == FX_S_OK);
            if (ret == FX_S_OK); //avoid warning
            FX_NS(com)::ComStr comStr(name);
            std::string str;
            comStr.copyTo(str);
            if (FX_NS(utility)::startsWithNoCase(str, std::string("processor.")))
            {
                m_sIdentifier = str;
            }
            else
            {
                m_sIdentifier = "processor." + str;
            }

            FX_NS(com)::FX_BSTR name2 = NULL;
            ret = pInterface->getDocType(&name2);
            FIRTEX_ASSERT2(ret == FX_S_OK);
            FX_NS(com)::ComStr var2(name2);
            var2.copyTo(m_sDocType);
        }

    public:
        /**
         * create a processor object
         * @return create a parser
         */
        virtual DocumentProcessor* create() const
        {
            IDocumentProcessorPlugin* pNewInterface = NULL;
            FX_NS(com)::FX_HRESULT ret = m_pInterface->clone(&pNewInterface);
            if (ret != FX_S_OK)
            {
                return NULL;
            }
            return new DocumentProcessorPlugin(pNewInterface);
        }

    private:
        IDocumentProcessorPlugin* m_pInterface;
    };

public:
    DocumentProcessorPlugin(IDocumentProcessorPlugin* pInterface);
    ~DocumentProcessorPlugin();

public:
    /// Initialize
    virtual void init(const FX_NS(document)::DocumentSchema* pDocSchema,
                      const FX_NS(processor)::DocumentTemplate* pTemplate);

    /**
     * Process document
     *
     * @param docSource source document to process
     */
    virtual void process(FX_NS(processor)::DocumentSource& docSource);

    /// Clone a new object
    virtual DocumentProcessor* clone() const;

private:
    IDocumentProcessorPlugin* m_pInterface;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(DocumentProcessorPlugin);

FX_NS_END

#endif //__FX_DOCUMENTPROCESSORPLUGIN_H
