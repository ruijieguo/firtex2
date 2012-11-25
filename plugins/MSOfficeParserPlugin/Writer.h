#ifndef __WRITER_H
#define __WRITER_H

class Analyze;

class Writer
{
public:
	Writer();
	Writer(Analyze* pAnalyze);
	virtual ~Writer(void);
public:
	void out_char(const char *chunk);
	virtual void output_paragraph(unsigned short int *buffer);
protected:
	int			wrap_margin;
	Analyze*	m_pAnalyze;
};

#endif
