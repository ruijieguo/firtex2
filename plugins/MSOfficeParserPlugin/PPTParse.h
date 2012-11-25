#ifndef __PPTPARSE_H
#define __PPTPARSE_H

class Analyze;
class PPTParse
{
public:
	PPTParse(Analyze* pAnalyze);
	~PPTParse(void);
public:
	void do_ppt(FILE *input) ;

	void process_item (int rectype, long reclen, FILE* input);
protected:
	inline void	appendChar(unsigned short int u);
	void	flush();
protected:
	Analyze* m_pAnalyze;

	unsigned short int	m_buffer[256];
	int		m_pos;
};

inline void	PPTParse::appendChar(unsigned short int u)
{
	if(m_pos >= 255)
		flush();
	m_buffer[m_pos++] = u;
}

#endif
