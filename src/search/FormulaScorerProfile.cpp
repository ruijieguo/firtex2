#include "firtex/search/FormulaScorerProfile.h"
#include "firtex/search/FormulaScorer.h"

FX_NS_DEF(search);

SETUP_LOGGER(search, FormulaScorerProfile);

FormulaScorerProfile::FormulaScorerProfile() 
{
}

FormulaScorerProfile::~FormulaScorerProfile() 
{
}

std::string FormulaScorerProfile::getProfileName() const
{
    return "ranking_formula";
}

ScorerPtr FormulaScorerProfile::createScorer() const
{
    FormulaScorer* pScorer = new FormulaScorer;
    pScorer->setParameter(m_sParams);
    return ScorerPtr(pScorer);
}

void FormulaScorerProfile::setParameters(const KeyValueMap& params)
{
    string sParams;
    for (KeyValueMap::const_iterator it = params.begin();
         it != params.end(); ++it)
    {
        sParams += it->first + "=";
        sParams += it->second + ";";
    }
    m_sParams = sParams;
}

FX_NS_END

