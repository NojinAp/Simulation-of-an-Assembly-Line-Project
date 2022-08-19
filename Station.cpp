/*
	==================================================
	Simulation of an Assembly Line Project
	==================================================
*/

#include <iomanip>
#include <string>

#include "Station.h"
#include "Utilities.h"

using namespace std;

namespace sdds
{
	size_t Station::m_widthField = 0;
	size_t Station::m_idGenerator = 0;

	Station::Station(const string& record)
	{
		Utilities util;
		size_t pos = 0;
		bool more = true;

		m_id = ++m_idGenerator;

		m_name= util.extractToken(record, pos, more);
		m_serialNum = stoi(util.extractToken(record, pos, more));
		m_quantity = stoi(util.extractToken(record, pos, more));

		if (m_widthField < util.getFieldWidth())
		{
			m_widthField = util.getFieldWidth();
		}

		m_desc = util.extractToken(record, pos, more);
	}

	const string& Station::getItemName() const
	{
		return m_name;
	}

	size_t Station::getNextSerialNumber()
	{
		return m_serialNum++;
	}

	size_t Station::getQuantity() const
	{
		return m_quantity;
	}

	void Station::updateQuantity()
	{
		if (m_quantity > 0)
		{
			m_quantity--;
		}
	}

	void Station::display(ostream& os, bool full) const
	{
		os << right << setfill('0') << setw(3) << m_id << " | ";
		os << left << setfill(' ') << setw(m_widthField) << m_name << "  | ";
		os << right << setfill('0') << setw(6) << m_serialNum << " | ";

		if (full)
		{
			os << setfill(' ') << right << setw(4) << m_quantity << " | ";
			os << m_desc;
		}

		os << endl;
	}
}