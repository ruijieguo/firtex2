#include "firtex/extension/collection/ConsoleProgressObserver.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(collection, ConsoleProgressObserver);

ConsoleProgressObserver::ConsoleProgressObserver(uint64_t nReportInterval) 
    : m_nProcessedCount(0)
    , m_nReportInterval(nReportInterval)
    , m_nLastProcessed(0)
{
}

ConsoleProgressObserver::~ConsoleProgressObserver() 
{
}

void ConsoleProgressObserver::step()
{
    if (m_nProcessedCount == 0)
    {
        FastMutex::Guard l(m_mutex);
        if (m_nProcessedCount == 0)
        {
            m_lastProccessTime.update();
            m_nProcessedCount = 1;
        }
    }

    ++m_lastProccessed;
    if (((long)m_lastProccessed % m_nReportInterval) == 0)
    {
        FastMutex::Guard l(m_mutex);
        
        m_nProcessedCount = (long)m_lastProccessed;

        cout << "Processed [" << m_nProcessedCount << "] items" << endl;
        Timestamp::TimeVal elapsed = m_lastProccessTime.microseconds();
        m_lastProccessTime.update();
        elapsed = m_lastProccessTime.microseconds() - elapsed;
        if (elapsed != 0)
        {
            cout << "Process speed: [" << ((m_nProcessedCount - m_nLastProcessed) * Timestamp::resolution())/(uint64_t)elapsed
                 << "] (document/second)." << endl;
        }
        else 
        {
            cout << "Process speed is unmeasurable." << endl;
        }
        m_nLastProcessed = m_nProcessedCount;
    }
}

void ConsoleProgressObserver::complete()
{
    cout << "Process is completed." << endl;
}

FX_NS_END

