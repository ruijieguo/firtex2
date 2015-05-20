#include "firtex/search/BM25TermScorer.h"

FX_NS_DEF(search);

SETUP_LOGGER(search, BM25TermScorer);

score_t BM25TermScorer::K1 = 2.0;
score_t BM25TermScorer::B = 0.75;


FX_NS_END

