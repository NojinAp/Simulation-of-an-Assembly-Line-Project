/*
	==================================================
	Simulation of an Assembly Line Project
	==================================================
*/

#include "Workstation.h"
#include "Station.h"
#include "CustomerOrder.h"

using namespace std;

namespace sdds
{
	deque<CustomerOrder> g_pending; 
	deque<CustomerOrder> g_completed; 
	deque<CustomerOrder> g_incomplete;

	Workstation::Workstation(const string& str) : Station(str) {};

	void Workstation::fill(ostream& os)
	{
		if (!m_orders.empty())
		{
			m_orders.front().fillItem(*this, os);
		}
	}

	bool Workstation::attemptToMoveOrder()
	{
		if (m_orders.size() > 0)
		{
			if (m_orders.front().isItemFilled(getItemName()) || getQuantity() <= 0) 
			{
				if (m_pNextStation) 
				{
					*m_pNextStation += std::move(m_orders.front());
					m_orders.pop_front();
					return true;
				}
				else
				{
					if (m_orders.front().isOrderFilled())
					{
						g_completed.push_back(std::move(m_orders.front()));
					}
					else
					{
						g_incomplete.push_back(std::move(m_orders.front()));
					}
					m_orders.pop_front();
					return true;
				}
			}
		}

		return false;
	}

	void Workstation::setNextStation(Workstation* station)
	{
		m_pNextStation = station;
	}

	Workstation* Workstation::getNextStation() const
	{
		return m_pNextStation;
	}

	void Workstation::display(ostream& os) const
	{
		os << getItemName() << " --> ";

		if (m_pNextStation != nullptr)
		{
			os << m_pNextStation->getItemName();
		}
		else
		{
			os << "End of Line";
		}

		os << endl;

	}

	Workstation& Workstation::operator+=(CustomerOrder&& newOrder)
	{
		m_orders.push_back(move(newOrder));

		return *this;
	}
}