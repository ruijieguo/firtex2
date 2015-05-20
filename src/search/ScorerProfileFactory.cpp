#include "firtex/search/ScorerProfileFactory.h"
#include "firtex/search/BM25ScorerProfile.h"
#include "firtex/search/FormulaScorerProfile.h"

using namespace std;

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, ScorerProfileFactory);

ScorerProfileFactory::ScorerProfileFactory() 
{
    doRegisterProfile("default", new BM25ScorerProfile());
    registerProfile(new BM25ScorerProfile());
    registerProfile(new FormulaScorerProfile());
}

ScorerProfileFactory::~ScorerProfileFactory() 
{
    for (ProfileMap::iterator it = m_profiles.begin();
         it != m_profiles.end(); ++it)
    {
        delete it->second;
    }
    m_profiles.clear();
}

void ScorerProfileFactory::registerProfile(ScorerProfile* pProfile)
{
    string sProfile = pProfile->getProfileName();
    doRegisterProfile(sProfile, pProfile);
}

void ScorerProfileFactory::doRegisterProfile(string sProfile, ScorerProfile* pProfile)
{
    if (m_profiles.find(sProfile) != m_profiles.end())
    {
        delete pProfile;
        FX_LOG(ERROR, "Register profile: [%s] FAILED: duplicated", 
               sProfile.c_str());
        return;
    }
    m_profiles.insert(make_pair(sProfile, pProfile));
}

const ScorerProfile* ScorerProfileFactory::getProfile(const std::string& sProfileName)
{
    ProfileMap::const_iterator it = m_profiles.find(sProfileName);
    if (it != m_profiles.end())
    {
        return it->second;
    }
    FX_LOG(ERROR, "Find profile: [%s] FAILED", sProfileName.c_str());
    return NULL;
}

FX_NS_END

