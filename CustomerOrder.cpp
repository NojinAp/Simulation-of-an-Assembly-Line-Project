/*
	==================================================
	Simulation of an Assembly Line Project
	==================================================
*/

#include <iomanip>
#include <algorithm>
#include <vector>

#include "CustomerOrder.h"
#include "Utilities.h"

using namespace std;

namespace sdds 
{
	size_t CustomerOrder::m_widthField = 0;

	CustomerOrder::CustomerOrder(const string str)
	{
		Utilities util;
		bool more = true;
		size_t next_pos = 0;

		m_name = util.extractToken(str, next_pos, more);
		m_product = util.extractToken(str, next_pos, more);
		m_cntItem = 0;

		auto start_pos = next_pos;

		while (more)
		{
			util.extractToken(str, next_pos, more);
			m_cntItem++;
		}

		m_lstItem = new Item* [m_cntItem];

		more = true;

		for (size_t i = 0; i < m_cntItem; i++)
		{
			m_lstItem[i] = new Item(util.extractToken(str, start_pos, more));
		}

		if (m_widthField < util.getFieldWidth())
		{
			m_widthField = util.getFieldWidth();
		}
	}

	CustomerOrder::CustomerOrder(const CustomerOrder& co)
	{
		throw string("Cannot make copies.");
	}

	CustomerOrder::CustomerOrder(CustomerOrder&& co) noexcept
	{
		*this = move(co);
	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& co) noexcept
	{
		if (this != &co)
		{
			for (size_t i = 0; i < m_cntItem; i++)
			{
				delete m_lstItem[i];
			}
			delete[] m_lstItem;

			m_lstItem = co.m_lstItem;
			m_name = co.m_name;
			m_product = co.m_product;
			m_cntItem = co.m_cntItem;

			co.m_name = "";
			co.m_product = "";
			co.m_lstItem = nullptr;
			co.m_cntItem = 0;
		}
		return *this;
	}

	CustomerOrder::~CustomerOrder()
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			delete m_lstItem[i];
		}

		delete[] m_lstItem;
		m_lstItem = nullptr;
	}

	bool CustomerOrder::isOrderFilled() const
	{
		bool filled = true;

		for (size_t i = 0; i < m_cntItem && filled; i++) 
		{
			if (m_lstItem[i]->m_isFilled == false)
			{
				filled = false;
			}
		}
		return filled;
	}

	bool CustomerOrder::isItemFilled(const string& itemName) const
	{
		bool found = false;
		bool flag = false;

		vector<Item*> matchedItems;
		for (size_t i = 0; i < m_cntItem; i++) 
		{
			if (m_lstItem[i]->m_itemName == itemName) 
			{
				found = true;
				matchedItems.push_back(m_lstItem[i]);
			}
		}

		if (found) 
		{
			flag = all_of(matchedItems.begin(), matchedItems.end(), [](const Item* it)
				{
					return it->m_isFilled == true; 
				});
		}

		else 
		{
			return true; //If the item doesn't exist in the order, this query returns true
		}

		return flag;
	}

	void CustomerOrder::fillItem(Station& station, ostream& os)
	{
		bool flag = true;

		for (size_t i = 0; i < m_cntItem && flag; i++) 
		{
			if (m_lstItem[i]->m_itemName == station.getItemName()) 
			{
				if (!m_lstItem[i]->m_isFilled) 
				{
					flag = false;
					if (station.getQuantity() > 0) 
					{
						station.updateQuantity();
						m_lstItem[i]->m_isFilled = true;
						m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
						os << "    Filled " << m_name << ", " << m_product << " [" << station.getItemName() << "]" << endl;
					}
					else
					{
						for (size_t j = 0; j < m_cntItem; j++) 
						{
							if (m_lstItem[j]->m_itemName == station.getItemName())
							{
								if (!m_lstItem[j]->m_isFilled)
								{
									os << "    Unable to fill " << m_name << ", " << m_product << " [" << station.getItemName() << "]" << endl;
								}
							}
						}
					}
				}
			}
		}
	}

	void CustomerOrder::display(ostream& os) const
	{
		os << m_name << " - " << m_product << endl;

		for (size_t i = 0; i < m_cntItem; i++)
		{
			os << "[" << right << setw(6) << setfill('0') << m_lstItem[i]->m_serialNumber << "] ";
			os << left << setw(m_widthField) << setfill(' ') << m_lstItem[i]->m_itemName << "   - ";
			if (m_lstItem[i]->m_isFilled)
			{
				os << "FILLED";
			}
			else
			{
				os << "TO BE FILLED";
			}
			os << endl;
		}
	}

}