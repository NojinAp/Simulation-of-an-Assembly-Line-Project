/*
	==================================================
	Simulation of an Assembly Line Project
	==================================================
*/

#ifndef SDDS_STATION_H
#define SDDS_STATION_H

#include <iostream>

namespace sdds
{
	class Station
	{
		int m_id{ 0 };
		std::string m_name{};
		std::string m_desc{};
		size_t m_serialNum{ 0 };
		size_t m_quantity{ 0 };

		static size_t m_widthField;
		static size_t m_idGenerator;

	public:
		Station() {}; //m_idGenerator++
		Station(const std::string& record); 
		const std::string& getItemName() const;
		size_t getNextSerialNumber();
		size_t getQuantity() const;
		void updateQuantity();
		void display(std::ostream& os, bool full) const;
	};
}

#endif