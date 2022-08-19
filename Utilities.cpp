/*
	==================================================
	Simulation of an Assembly Line Project
	==================================================
*/

#include "Utilities.h"

using namespace std;

namespace sdds
{
	char Utilities::m_delimiter;

	void Utilities::setFieldWidth(size_t newWidth)
	{
		m_widthField = newWidth;
	}

	size_t Utilities::getFieldWidth() const
	{
		return m_widthField;
	}

	string Utilities::extractToken(const string& str, size_t& next_pos, bool& more)
	{
		string token;
		size_t delimitPos = str.find(m_delimiter, next_pos);

		if (more)
		{
			if (delimitPos == string::npos)
			{
				more = false;
			}
			else
			{
				more = true;
			}

			token = str.substr(next_pos, delimitPos - next_pos);

			token.erase(0, token.find_first_not_of(' '));
			token.erase(token.find_last_not_of(' ') + 1);

			next_pos = delimitPos + 1;

			if (token.empty())
			{
				more = false;
				throw string("No Token");
			}
			else if (m_widthField < token.size())
			{
				setFieldWidth(token.size());
			}
		}

		return token;
	}

	void Utilities::setDelimiter(char newDelimiter)
	{
		m_delimiter = newDelimiter;
	}

	char Utilities::getDelimiter()
	{
		return m_delimiter;
	}
}