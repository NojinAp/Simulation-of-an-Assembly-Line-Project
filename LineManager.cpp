/*
    ==================================================
    Simulation of an Assembly Line Project
    ==================================================
*/

#include <fstream>
#include <algorithm>
#include <string>

#include "Utilities.h"
#include "LineManager.h"

using namespace std;

namespace sdds 
{
    LineManager::LineManager(const string& file, const vector<Workstation*>& stations)
    {
        ifstream fs(file);

        if (!fs)
        {
            throw string("Could not open file");
        }

        string record;
        vector<string> records;

        while (!fs.eof())
        {
            getline(fs, record);
            records.push_back(record);
        }

        fs.close();

        string active;
        string next;
        Utilities helper;
        vector<string> nexts;

        for (auto& line : records)
        {
            size_t next_pos = 0;
            bool more = true;
            active = helper.extractToken(line, next_pos, more);
            next = more ? helper.extractToken(line, next_pos, more) : "";
            auto iterActive = find_if(stations.begin(), stations.end(), [=](const Workstation* ptr) {return ptr->getItemName() == active; });
            auto iterNext = find_if(stations.begin(), stations.end(), [=](const Workstation* ptr) {return ptr->getItemName() == next; });

            if (iterActive != stations.end())
            {
                m_activeLine.push_back(*iterActive);
                (*iterActive)->setNextStation((iterNext != stations.end()) ? *iterNext : nullptr);
            }

            if (next != "") nexts.push_back(next);
        }

        for (auto& ele : m_activeLine)
        {
            if (none_of(nexts.begin(), nexts.end(), [&](const string str) {return str == ele->getItemName(); }))
            {
                m_firstStation = ele;
            }
        }
        m_cntCustomerOrder = g_pending.size();
    }

    void LineManager::reorderStations()
    {
        const Workstation* ws = m_firstStation;
        size_t i = 0;

        do
        {
            m_activeLine[i++] = const_cast<Workstation*>(ws);
            ws = ws->getNextStation();
        } while (ws != nullptr);
    }

    bool LineManager::run(ostream& os)
    {
        static size_t iterations = 0;
        os << "Line Manager Iteration: " << ++iterations << endl;

        if (!g_pending.empty())
        {
            *m_firstStation += move(g_pending.front());
            g_pending.pop_front();
        }

        for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* ws) 
            {
            ws->fill(os);
            });
        for_each(m_activeLine.begin(), m_activeLine.end(), [](Workstation* ws)
            {
            ws->attemptToMoveOrder();
            });

        return m_cntCustomerOrder == (g_completed.size() + g_incomplete.size());
    }

    void LineManager::display(ostream& os) const
    {
        if (!m_activeLine.empty())
        {
            for (auto& i : m_activeLine)
            {
                i->display(os);
            }
        }
    }
}