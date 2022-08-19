/*
	==================================================
	Simulation of an Assembly Line Project
	==================================================
*/

#ifndef SDDS_WORKSTATION_H
#define SDDS_WORKSTATION_H

#include <queue>

#include "Station.h"
#include "CustomerOrder.h"

namespace sdds
{
	extern std::deque<CustomerOrder> g_pending; //orders to be placed onto the assembly line at the first station
	extern std::deque<CustomerOrder> g_completed; //orders that have been removed from the last station and have been completely filled
	extern std::deque<CustomerOrder> g_incomplete; //orders that have been removed from the last station and could not be filled completely

	class Workstation : public Station
	{
		std::deque<CustomerOrder> m_orders;
		Workstation* m_pNextStation{ nullptr };

	public:
		Workstation() {};
		Workstation(const std::string& str);
		void fill(std::ostream& os);
		bool attemptToMoveOrder();
		void setNextStation(Workstation* station);
		Workstation* getNextStation() const;
		void display(std::ostream& os) const;
		Workstation& operator+=(CustomerOrder&& newOrder);
	};
}

#endif