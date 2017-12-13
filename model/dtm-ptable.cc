#include "dtm-ptable.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <algorithm>
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("DtmTable");


namespace ns3 {
namespace dtm {

/*
  dtm position table
*/

PositionTable::PositionTable ()
{
  m_txErrorCallback = MakeCallback (&PositionTable::ProcessTxError, this);
  m_entryLifeTime = Seconds (25); //FIXME fix parameter to hello message timer

  m_threshold = 0;

}

Time 
PositionTable::GetEntryUpdateTime (uint32_t id)
{
  if (id == 0)
    {
      return Time (Seconds (0));
    }
  std::map<uint32_t, std::tuple<Vector, Vector, Time> >::iterator i = m_table.find (id);
  return std::get<2> (i->second);
}

/**
 * \brief Adds entry in position table and delete earlier entry if already present
 */
void 
PositionTable::AddEntry (uint32_t id, Vector position, Vector velocity)
{
  std::map<uint32_t, std::tuple<Vector, Vector, Time> >::iterator i = m_table.find (id);
  if (i != m_table.end () || id == (i->first))
    {
      m_table.erase (id);
    }

  m_table.insert (std::make_pair (id, std::make_tuple (position, velocity, Simulator::Now ())));

  CalculateThreshold ();

}

/**
 * \brief Deletes entry in position table
 */
void 
PositionTable::DeleteEntry (uint32_t id)
{
  std::map<uint32_t, std::tuple<Vector, Vector, Time> >::iterator i = m_table.find (id);
  if (i != m_table.end ())
    {
        m_table.erase (id);
        CalculateThreshold ();
    }
}

/**
 * \brief Gets position from position table
 * \param id uint32_t to get position from
 * \return Position of that id or NULL if not known
 */
Vector 
PositionTable::GetPosition (uint32_t id)
{

  NodeList::Iterator listEnd = NodeList::End ();
  for (NodeList::Iterator i = NodeList::Begin (); i != listEnd; i++)
    {
      Ptr<Node> node = *i;
      if (node->GetId () == id)
        {
          return node->GetObject<MobilityModel> ()->GetPosition ();
        }
    }
  return PositionTable::GetInvalidPosition ();

}

/**
 * \brief Checks if a node is a neighbour
 * \param id uint32_t of the node to check
 * \return True if the node is neighbour, false otherwise
 */
bool
PositionTable::isNeighbour (uint32_t id)
{

 std::map<uint32_t, std::tuple<Vector, Vector, Time> >::iterator i = m_table.find (id);
  if (i != m_table.end () || id == (i->first))
    {
      return true;
    }

  return false;
}


/**
 * \brief remove entries with expired lifetime
 */
void 
PositionTable::Purge ()
{

  if(m_table.empty ())
    {
      return;
    }

  std::list<uint32_t> toErase;

  std::map<uint32_t, std::tuple<Vector, Vector, Time> >::iterator i = m_table.begin ();
  std::map<uint32_t, std::tuple<Vector, Vector, Time> >::iterator listEnd = m_table.end ();
  
  for (; !(i == listEnd); i++)
    {

      if (m_entryLifeTime + GetEntryUpdateTime (i->first) <= Simulator::Now ())
        {
          toErase.insert (toErase.begin (), i->first);

        }
    }
  toErase.unique ();

  std::list<uint32_t>::iterator end = toErase.end ();

  for (std::list<uint32_t>::iterator it = toErase.begin (); it != end; ++it)
    {

      //m_table.erase (*it);
      PositionTable::DeleteEntry (*it);
    }
}

/**
 * \brief clears all entries
 */
void 
PositionTable::Clear ()
{
  m_table.clear ();

  m_threshold = 0;
}

/**
 * \ProcessTxError
 */
void 
PositionTable::ProcessTxError (WifiMacHeader const & hdr) /// FIXME complete this function
{
}

/**
 * \brief Returns true if is in search for destination
 */
bool 
PositionTable::IsInSearch (uint32_t id)
{
  return false;
}

bool 
PositionTable::HasPosition (uint32_t id)
{
  return true;
}


/// Calculate and update distance Threshold Mc 
void 
PositionTable::CalculateThreshold () /// FIXME to complete
{
  if (m_table.empty ())
    {
      /// FIXME if needed
    }
  m_threshold = 0.80 - 0.95 * exp(-0.06 * (double) m_table.size ());
}

/*
void
PositionTable::CalculateSpatialDistribution ()
{
  std::map<uint32_t, std::tuple<Vector,Vector,Time>>::const_iterator i = m_table.begin ();

  m_spatialDist.x = 0;
  m_spatialDist.y = 0;

  if (!(m_table.empty ()))
    {
      double sumx = 0;
      double sumy = 0;

      for (; i != m_table.end (); i++)
        {
          sumx += std::get<0> (i->second).x;
          sumy += std::get<0> (i->second).y;      
        }

      m_spatialDist.x =  ((double) 1/m_table.size ()) * sumx;
      m_spatialDist.y =  ((double) 1/m_table.size ()) * sumy;  
    }
}
*/

void 
PositionTable::Print (std::ostream & os) const
{
  std::map<uint32_t, std::tuple<Vector, Vector, Time>>::const_iterator i = m_table.begin ();
  while (i != m_table.end ())
    {
      os << "\n id : " << i->first;
      i++;
    }
  os << "\n Threshold : " << m_threshold;
}


std::ostream & operator<< (std::ostream & os, PositionTable const & h)
{
  h.Print(os);
  return os;
}

}   // dtm
} // ns3
