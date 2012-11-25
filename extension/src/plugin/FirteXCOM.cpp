//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/9
//
#include "firtex/extension/plugin/FirteXCOM.h"
//#ifdef _DEBUG
//	#pragma comment(lib, "comD")  
//#else
//	#pragma comment(lib, "com")  
//#endif

FX_NS_DEF(plugin)

// {6D0B428B-3779-4d74-85AE-3F4DB4E29AE5}
FX_NS(com)::FX_IID const IFieldTag::iid = { 0x6d0b428b, 0x3779, 0x4d74, { 0x85, 0xae, 0x3f, 0x4d, 0xb4, 0xe2, 0x9a, 0xe5 } };
// {F3EFAAE3-BD25-4f2a-835D-D36367EF44AD}
FX_NS(com)::FX_IID const IRawDocument::iid = { 0xf3efaae3, 0xbd25, 0x4f2a, { 0x83, 0x5d, 0xd3, 0x63, 0x67, 0xef, 0x44, 0xad } };
// {C0CCA7C8-79FE-40d6-AED8-1FD8FABD6F6A}
FX_NS(com)::FX_IID const IDocumentSource::iid = { 0xc0cca7c8, 0x79fe, 0x40d6, { 0xae, 0xd8, 0x1f, 0xd8, 0xfa, 0xbd, 0x6f, 0x6a } };
// {2E15293F-ED12-44f0-B953-35DA4D51E0EC}
FX_NS(com)::FX_IID const IDocumentTemplate::iid = { 0x2e15293f, 0xed12, 0x44f0, { 0xb9, 0x53, 0x35, 0xda, 0x4d, 0x51, 0xe0, 0xec } };
// {3EF8289A-9514-4c24-B6C2-4A6A595C9333}
FX_NS(com)::FX_IID const IDocumentProcessorPlugin::iid = { 0x3ef8289a, 0x9514, 0x4c24, { 0xb6, 0xc2, 0x4a, 0x6a, 0x59, 0x5c, 0x93, 0x33 } };
// {09B42270-8ACD-4e8a-B125-A79B46003F5D}		
FX_NS(com)::FX_IID const IDocumentSchema::iid = { 0x9b42270, 0x8acd, 0x4e8a, { 0xb1, 0x25, 0xa7, 0x9b, 0x46, 0x0, 0x3f, 0x5d } };
// {C92FA9F4-86FD-48c1-AB00-B16C78D2983F}
FX_NS(com)::FX_IID const IDocument::iid = { 0xc92fa9f4, 0x86fd, 0x48c1, { 0xab, 0x0, 0xb1, 0x6c, 0x78, 0xd2, 0x98, 0x3f } };
// {E6F0F10C-DF0B-411a-9A3E-EA2BCD4DF256}
FX_NS(com)::FX_IID const ITokenView::iid = { 0xe6f0f10c, 0xdf0b, 0x411a, { 0x9a, 0x3e, 0xea, 0x2b, 0xcd, 0x4d, 0xf2, 0x56 } };
// {8AD182C7-E4BD-41b5-B73E-E78FEB575A4C}
FX_NS(com)::FX_IID const ITokenSource::iid = { 0x8ad182c7, 0xe4bd, 0x41b5, { 0xb7, 0x3e, 0xe7, 0x8f, 0xeb, 0x57, 0x5a, 0x4c } };
// {7D7D876B-CCE4-4fe0-8389-A20106A828A3}
FX_NS(com)::FX_IID const IAnalyzerPlugin::iid = { 0x7d7d876b, 0xcce4, 0x4fe0, { 0x83, 0x89, 0xa2, 0x1, 0x6, 0xa8, 0x28, 0xa3 } };

FX_NS_END
