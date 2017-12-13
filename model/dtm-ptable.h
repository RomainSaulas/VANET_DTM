#ifndef DTM_PTABLE_H
#define DTM_PTABLE_H

#include <map>
#include <cassert>
#include <stdint.h>
#include "ns3/ipv4.h"
#include "ns3/timer.h"
#include <sys/types.h>
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/mobility-model.h"
#include "ns3/vector.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/random-variable-stream.h"
#include <complex>

namespace ns3 {
namespace dtm {

/*
 * \ingroup dtm
 * \brief Position table used by DTM
 */
class PositionTable
{
public:
  /// c-tor
  PositionTable ();

  /**
   * \brief Gets the last time the entry was updated
   * \param id uint32_t to get time of update from
   * \return Time of last update to the position
   */
  Time GetEntryUpdateTime (uint32_t id);

  /**
   * \brief Adds entry in position table
   */
  void AddEntry (uint32_t id, Vector position, Vector velocity);

  /**
   * \brief Deletes entry in position table
   */
  void DeleteEntry (uint32_t id);

  /**
   * \brief Gets position from position table
   * \param id uint32_t to get position from
   * \return Position of that id or NULL if not known
   */
  Vector GetPosition (uint32_t id);

  /**
   * \brief Checks if a node is a neighbour
   * \param id uint32_t of the node to check
   * \return True if the node is neighbour, false otherwise
   */
  bool isNeighbour (uint32_t id);

  /**
   * \brief remove entries with expired lifetime
   */
  void Purge ();

  /**
   * \brief clears all entries
   */
  void Clear ();

  /**
   * \Get Callback to ProcessTxError
   */
  Callback<void, WifiMacHeader const &> GetTxErrorCallback () const
  {
    return m_txErrorCallback;
  }

  bool IsInSearch (uint32_t id);

  bool HasPosition (uint32_t id);

  static Vector GetInvalidPosition ()
  {
    return Vector (-1, -1, 0);
  }

  double GetThreshold () const {
    return m_threshold;
  }

  void SetThreshold (double threshold) 
  {
    m_threshold = threshold;
  }

  /*Vector GetSpatialDistribution () const {
    return m_spatialDist;
  }

  void SetSpatialDistribution (Vector spatialDist) 
  {
    m_spatialDist = spatialDist;
  }*/

  void Print (std::ostream & os) const;

private:
  Time m_entryLifeTime;
  std::map<uint32_t, std::tuple<Vector, Vector, Time>> m_table;
  // TX error callback
  Callback<void, WifiMacHeader const &> m_txErrorCallback;

  double m_threshold;        ///< Distance to mean Threshold
  //Vector m_spatialDist;     ///< Spatial Distribution calculated over neighbor position


  // Process layer 2 TX error notification
  void ProcessTxError (WifiMacHeader const&);


  /// Calculate and update distance Threshold Mc 
  void CalculateThreshold ();

};

std::ostream & operator<< (std::ostream & os, PositionTable const & h);

}   // dtm
} // ns3
#endif /* DTM_PTABLE_H */
