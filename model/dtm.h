/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DTM_H
#define DTM_H

#include "dtm-packet.h"
#include "dtm-ptable.h"

#include "ns3/node.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"

#include "ns3/ipv4.h"
#include "ns3/ip-l4-protocol.h" 
#include "ns3/icmpv4-l4-protocol.h" 

#include "ns3/mobility-model.h"
//#include "dtm-rqueue.h"

#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/location-service.h"
#include "ns3/god.h"

#include "ns3/vector.h"

#include <map>
#include <complex>


NS_LOG_COMPONENT_DEFINE ("DtmRoutingProtocol");


namespace ns3 {
namespace dtm {

/**
 * \ingroup dtm 
 *
 * \brief DTM routing protocol
 */
class RoutingProtocol: public Ipv4RoutingProtocol
{
public:
	static TypeId GetTypeId (void);
	static const uint32_t DTM_PORT;

	/// c-tor
	RoutingProtocol ();
	virtual ~RoutingProtocol ();
	virtual void DoDispose ();

	///\name From Ipv4RoutingProtocol
	//
	//
  virtual void SetIpv4 (Ptr<Ipv4> ipv4);
	virtual void sendHello ();
	virtual bool IsMyOwnAddress (Ipv4Address src); 
	virtual void sendWarning (Ipv4Address broadcastAddr);

	PositionTable GetNeighbors ()
	{
		return m_neighbors;
	}

	Ptr<Ipv4> m_ipv4;

	Ptr<NetDevice> m_lo;

	/// Broadcast ID
	uint32_t m_requestId;
	/// Request sequence number
	//uint32_t m_seqNo;

	Time HelloInterval;

	virtual void PrintRoutingTable (ns3::Ptr<ns3::OutputStreamWrapper>) const
	{
		return;
	}

	//void RoutingProtocol::Forward (Ptr<Packet> packet);

	//typedef Callback<void, Ptr<Packet>> BroadcastForward;

	//bool ForwardPacketFromQueue (uint32_t packetId);

	Ptr<Packet> GenerateHello (uint32_t id = 0, uint64_t originPosx = 0, uint64_t originPosy = 0, uint64_t speed = 0);

	//Ptr<Packet>	GenerateWarning (uint32_t id);

	void SetHelloIntervalTimer (Timer helloTimer) 
	{
		HelloIntervalTimer = helloTimer;
	}
	Timer GetHelloIntervalTimer () const
	{
		return HelloIntervalTimer;
	}

private:
	/// Start protocol operation
	void Start ();

	/// If next hop found and valid forward packet
	void HelloTimerExpire ();

	/// Queue packet and set Backoff timer
	//Ptr<NetDevice> Loopback (const Ipv4Header & header, Ptr<NetDevice> oif);

	/// If distance to mean higher than threshold
	bool Forwarding (Ptr<const Packet> p, const Ipv4Header & header);

	/// Calls sendPacketFromQueue and re-schedules
	//void CheckQueue ();

	/// Calculate distance to mean M
	uint64_t CalculateDTM ();

	/// Receive DTM packet
	void	RecvDTM (Ptr<Socket> socket);

	///uint32_t MaxQueueLen;		///< The maximum number of packet stocked in queue
	//Time MaxQueueTime;			///< The maximum period of time a packet can be in queue 
	//RequestQueue m_queue;

	Timer HelloIntervalTimer;

	PositionTable m_neighbors;
	//std::list<uint32_t> m_queuedId;
};


}
}

#endif /* DTMROUTINGPROTOCOL_H */

