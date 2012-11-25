#include "firtex/analyzer/DoubleArrayTrie.h"

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, DoubleArrayTrie);

DoubleArrayTrie::DoubleArrayTrie()
    : m_header(NULL)
    , m_states(NULL)
    , m_pMMapFile(NULL)
{
}

DoubleArrayTrie::DoubleArrayTrie(size_t nNumElems)
    : m_header(NULL)
    , m_states(NULL)
    , m_pMMapFile(NULL)
{
    m_header = new Header;
    m_header->max = 0;
    m_header->min = 0xffffff;
    m_header->sum = 0;
    m_header->num = 0;
    m_header->insertCount = 0;
    strncpy(m_header->magic, "dbarray", MAGIC_SIZE);

    resizeState(nNumElems);
    m_lastState = INIT_STATE;
}

DoubleArrayTrie::~DoubleArrayTrie()
{
    if (m_pMMapFile) 
    {
        delete m_pMMapFile;
    } 
    else 
    {
        delete m_header;
        free(m_states);
    }
}

void DoubleArrayTrie::load(const tstring& sFileName)
{
    m_pMMapFile = new MMapFile();
    m_pMMapFile->open(sFileName, MMapFile::AM_READ);
    m_header = (Header *)m_pMMapFile->begin();
    m_states = (State *)(m_pMMapFile->begin() + sizeof(Header));
}

void DoubleArrayTrie::buildFromFile(const tstring& sFileName)
{
    char buffer[4096];
    int nFreq;

    FILE* fp = fopen(sFileName.c_str(), "r");

    FX_LOG(INFO, "Building trie lexicon: [%s]", sFileName.c_str());

    size_t i = 0;
    while (fscanf(fp, "%s %d[^\r\n]", buffer, &nFreq) != EOF)
    {
        insert(buffer, nFreq);
        
        if (i % 1000 == 0)
        {
            FX_LOG(INFO, "The [%lu]th terms processed", (uint32_t)i);
        }
        i++;
    }

    fclose(fp);
    FX_LOG(INFO, "[%lu] terms processed", (uint32_t)i);
}

DoubleArrayTrie::ValueType
DoubleArrayTrie::findBase(ValueType *key, ValueType max, ValueType min)
{
    bool found;
    ValueType i, *p;

    for (i = m_lastState, found = false; !found;) 
    {
        i++;
        if (i + key2state(max) >= m_header->num) 
            resizeState(key2state(max));
        if (getCheck(i + key2state(min)) <= 0 &&
            getCheck(i + key2state(max)) <= 0) 
        {
            found = true;
            for (p = key; *p > -1; p++) 
            {
                if (getCheck(i + key2state(*p)) > 0) 
                {
                    found = false;
                    break;
                }
            }
        }
    }
    m_lastState = i;
    return i;
}

DoubleArrayTrie::ValueType
DoubleArrayTrie::relocate(ValueType stand, ValueType s,
                          ValueType *key, ValueType max,
                          ValueType min)
{
    ValueType old, neo, i, *p;
    ValueType ancestor[ALPHABET_SIZE] = {0};

    FIRTEX_ASSERT2(s > 0 && s < m_header->num);
    old = getBase(s);
    neo = findBase(key, max, min);
    for (i = 0; key[i] > -1; i++) 
    {
        ValueType t = key2state(key[i]);

        if (getCheck(old + t) != s) continue;
        setBase(neo + t, getBase(old + t));
        setCheck(neo + t, getCheck(old + t));
        findAccepts(old + t, ancestor, NULL, NULL);
        for (p = ancestor; *p > -1; p++) 
            setCheck(getBase(old + t) + key2state(*p), neo + t);
        if (stand == old + t) 
        {
            stand = neo + t; 
        }
        setBase(old + t, 0);
        setCheck(old + t, 0);
        FIRTEX_ASSERT2(getCheck(old + t) == 0 && getBase(old + t) == 0);
    }
    setBase(s, neo);
    return stand;
}

DoubleArrayTrie::ValueType
DoubleArrayTrie::createTransition(ValueType s, ValueType ch)
{
    ValueType t, n, m, ns[ALPHABET_SIZE] = {0}, ms[ALPHABET_SIZE] = {0};
    ValueType max[] = {0, 0}, min[] = {0, 0};

    FIRTEX_ASSERT2(s > 0 && s < m_header->num);
    t = forceNext(s, ch);
    if (getBase(s) > 0 && getCheck(t) <= 0) 
    {
        //Nothing to do
    } 
    else 
    {
        n = findAccepts(s, ns, &max[0], &min[0]);
        m = (getCheck(t) == 0) ? 0 
            : findAccepts(getCheck(t), ms, &max[1], &min[1]);
        if (m > 0 && n + 1 > m) 
        {
            s = relocate(s, getCheck(t), ms, max[1], min[1]); 
        } 
        else 
        {
            ns[n++] = ch;
            ns[n] = -1;
            if (ch > max[0] || max[0] == 0) max[0] = ch;
            if (ch < min[0] || min[0] == 0) min[0] = ch;
            s = relocate(s, s, ns, max[0], min[0]);
        }
        t = forceNext(s, ch);
        FIRTEX_ASSERT2(getBase(s) > 0 && getCheck(t) <= 0);
    }
    setCheck(t, s);

    return t;
}

void DoubleArrayTrie::insert(const char *szKey, ValueType val)
{
    const char *p;
    ValueType s, t;

    FIRTEX_ASSERT2(val >= 0);
    FIRTEX_ASSERT2(szKey != NULL);
    for (p = szKey, s = 1;; p++) 
    {
        t = forceForward(s, (unsigned char)*p);
        if (t == 0) 
        {
            for (;; p++) 
            {
                s = createTransition(s, (unsigned char)*p);
                if (*p == '\0') break;
            }
            break;
        }
        s = t; /* move to next state */
        if (*p == '\0') break;
    }
    setBase(s, val);
    updateStatistics(val);
}

DoubleArrayTrie::ValueType
DoubleArrayTrie::search(const char *szKey) const
{
    ValueType state = 1, target;
    const char* p = szKey;
    do
    {
        target = forward(state, (unsigned char)*p);
        if (target == 0)
        {
            return 0;
        }
        state = target;
    } while (*p++ != '\0');

    return state? getBase(state) : 0;
}

DoubleArrayTrie::ValueType
DoubleArrayTrie::search(const char* szKey, size_t length) const
{
    ValueType state = 1, target;
    const char* p = szKey;
    size_t i = length;
    do
    {
        target = forward(state, (unsigned char)*p);
        if (target == 0)
        {
            return 0;
        }
        state = target;
        --i;
        ++p;
    } while (i > 0);

    state = forward(state, '\0');
    return state? getBase(state) : 0;
}

#define IF_AND_RETURN(state, end)                       \
    if (state == 0) {*keyEnd = szKey; return 0;}        \
    else {*keyEnd = end; return getBase(state);}

DoubleArrayTrie::ValueType
DoubleArrayTrie::search(const char* szKey, const char** keyEnd) const
{
    ValueType stack[128];
    ValueType stackTail = 0;

    ValueType state = 1, target;
    const char* p = szKey;
    do
    {
        target = forward(state, (unsigned char)*p);
        if (target == 0)
        {
            target = forward(state, '\0');
            while (target == 0 && stackTail > 0)
            {
                target = forward(stack[--stackTail], '\0');
                --p;
            }
            IF_AND_RETURN(target, p);
        }
        state = target;
        stack[stackTail++] = state;
    } while (*p++ != '\0');

    IF_AND_RETURN(state, p - 1);
}

DoubleArrayTrie::ValueType
DoubleArrayTrie::search(const char* szKey,
                        size_t length,
                        const char** keyEnd) const
{
    ValueType stack[128];
    ValueType stackTail = 0;

    ValueType state = 1, target;
    const char* p = szKey;
    size_t i = length;
    do
    {
        target = forward(state, (unsigned char)*p);
        if (target == 0)
        {
            target = forward(state, '\0');
            while (target == 0 && stackTail > 0)
            {
                target = forward(stack[--stackTail], '\0');
                --p;
            }
            //FIRTEX_ASSERT2(stackTail >= 0 && target > 0);

            IF_AND_RETURN(target, p);
        }
        stack[stackTail++] = state;
        state = target;
        --i;
        ++p;
    } while (i > 0);

    target = forward(state, '\0');
    IF_AND_RETURN(target, p);
}

#undef IF_AND_RETURN

void DoubleArrayTrie::traversal()
{
    doTraversal(INIT_STATE, 0);
}

void DoubleArrayTrie::doTraversal(ValueType s, ValueType off)
{
    ValueType *p, key[ALPHABET_SIZE];
    ValueType t;

    FIRTEX_ASSERT2(s > 0 && s < m_header->num);
    if (findAccepts(s, key, NULL, NULL) > 0) 
    {
        for (p = key; *p > -1; p++) 
        {
            m_traveralBuff[off] = (char)*p;
            t = forceNext(s, *p);
            FIRTEX_ASSERT2(t < m_header->num && t > 0);
            doTraversal(t, off + 1);
        }
    } 
    else 
    {
        onTraversalFinish(s, off);
    }
}

void DoubleArrayTrie::save(const tstring& sFileName)
{
    FILE *fp = fopen(sFileName.c_str(), "w+");
    if (fp == NULL)
    {
        FIRTEX_THROW(FileIOException, "Can not write to file: [%s]",
                     sFileName.c_str());
    }

    fwrite(m_header, sizeof(Header), 1, fp);
    fwrite(m_states, m_header->num * sizeof(State), 1, fp);
    fclose(fp);
}

FX_NS_END

