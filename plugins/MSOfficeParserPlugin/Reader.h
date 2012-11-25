#ifndef __READER_H
#define __READER_H

#define PARAGRAPH_BUFFER 262144

class Analyze;
class Reader
{
public:
	Reader(Analyze* pAnalyze);
	~Reader(void);
public:
	void copy_out (FILE *f,char *header);
	int process_file(FILE *f,long stop);
protected:
	unsigned short int buffer[PARAGRAPH_BUFFER];
	Analyze*	m_pAnalyze;	
};

#endif
