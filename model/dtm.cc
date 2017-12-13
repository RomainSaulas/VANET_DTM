/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "dtm.h"

#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/random-variable-stream.h"
#include "ns3/inet-socket-address.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/wifi-net-device.h"
#include "ns3/adhoc-wifi-mac.h"
#include <algorithm>
#include <limits>


namespace ns3 {
namespace dtm {
/* ... */

NS_OBJECT_ENSURE_REGISTERED (RoutingProtocol);

/// UDP Port for DTM control traffic
const uint32_t RoutingProtocol::DTM_PORT = 666;

RoutingProtocol::RoutingProtocol ()
	: HelloInterval(Seconds (10))/*,
		CheckQueueTimer (Seconds(25)),
		MaxQueueTime (Seconds (25)),
		MaxQueueLen (64),
		HelloIntervalTimer (Timer::CANCEL_ON_DESTROY),
		m_queue (MaxQueueLen, MaxQueueTime) /// to complete
	*/
{
	m_neighbors = PositionTable ();
}

TypeId
RoutingProtocol::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::dtm::RoutingProtocol")
		.SetParent<Ipv4RoutingProtocol> ()
		.AddConstructor<RoutingProtocol> ()
		.AddAttribute ("HelloInterval", "HELLO messages emsission interval.",
										TimeValue (Seconds (1)),
										MakeTimeAccessor (&RoutingProtocol::HelloInterval),
										MakeTimeChecker ())
		/*.AddAttribute ("PerimeterMode", "Indicates if PerimeterMode is enabled",
									 BooleanValue (false),
								 	 MakeBooleanAccessor (&RoutingProtocol::PerimeterMode),
								 	 MakeBooleanChecker ()) */
	;
	return tid;
}

RoutingProtocol::~RoutingProtocol ()
{
}

/*void
RoutingProtocol::DoDispose ()
{
	m_ipv4 = 0;
	Ipv4RoutingProtocol::DoDispose ();
}*/

void
RoutingProtocol::RecvDTM (Ptr<Socket> socket)
{
	NS_LOG_INFO ("ReceivePacket");\

   Ptr<Packet> packet = socket->Recv();

   
   dtm::TypeHeader tHeader (dtm::DTM_HELLO);
   packet->RemoveHeader (tHeader);
   if (!tHeader.IsValid ())
      {
      NS_LOG_DEBUG ("DTM message " << packet->GetUid () << " with unknown type received: " << tHeader.Get () << ". Ignored");
         return;
      }
   switch (tHeader.Get ())
      {
         case (dtm::DTM_HELLO):
            {
               dtm::HelloHeader hHeader;
               packet->RemoveHeader (hHeader);
               
               uint32_t id;
               Vector position;
               Vector speed;
   
               id = hHeader.GetId ();
               position.x = hHeader.GetOriginPosx ();
               position.y = hHeader.GetOriginPosy ();
               speed.x = hHeader.GetSpeedx ();
               speed.y = hHeader.GetSpeedy ();

               NS_LOG_INFO("Receive Hello:");
               NS_LOG_INFO (" Id " << id 
                        << " X " << position.x
                        << " Y " << position.y
                        << " Speed X " << speed.x
                        << " Speed Y " << speed.y);

               //RoutingProtocol::GetNeighbors ().AddEntry (id, position, speed);
   
               break;
            }
         /*case (dtm::DTM_WARNING):
            {// To Code
               WarningHeader wHeader;
               packet->PeekHeader (wHeader);
   
               if (m_queue.find (packet.GetUid ()))
                  {
                     m_queue.Dequeue ();
                  }
               else
                  {
                     QueueEntry en = QueueEntry (packet, RoutingProtocol::Forward, wHeader);
   
                     m_queue.Enqueue (&en);
                  }
   
               break;
            }*/
         default:
         NS_LOG_DEBUG ("DTM message " << packet->GetUid () << " with unknown type received: " << tHeader.Get () << ". Ignored");
      }
}

/*
void
RoutingProtocol::CheckQueue ()
{
	CheckQueueTimer.Cancel ();

	std::list<uint32_t> toRemove;

	for (std::list<uint32_t>::iterator i = m_queuedId.begin (); i != m_queuedId.end (); ++i)
    {
      if (ForwardPacketFromQueue (*i))
        {
          //Insert in a list to remove later
          toRemove.insert (toRemove.begin (), *i);
        }
    }

  //remove all that are on the list
  for (std::list<uint32_t>::iterator i = toRemove.begin (); i != toRemove.end (); ++i)
    {
      m_queuedId.remove (*i);
    }

  if (!m_queuedId.empty ()) //Only need to schedule if the queue is not empty
    {
      CheckQueueTimer.Schedule (Time ("500ms"));
    }
}
*/

/*
bool
RoutingProtocol::ForwardPacketFromQueue (uint32_t packetId)
{
	QueueEntry en = m_queue.Get (packetId);
	IsExpired pred;

	if (en != NULL && pred (en)) 
		{
			en.Forward ();
			return true;
		}

	return false;
}
*/

Ptr<Packet>
RoutingProtocol::GenerateHello (uint32_t id, uint64_t originPosx, uint64_t originPosy, uint64_t speed)
{
	Ptr<Packet> packet;

	HelloHeader hHeader = HelloHeader(id, originPosx, originPosy, speed);
	packet->AddHeader(hHeader);

	TypeHeader tHeader = TypeHeader(DTM_HELLO);
	packet->AddHeader(tHeader);

	return packet;
}

/*
Ptr<Packet>
RoutingProtocol::GenerateWarning (uint32_t id)
{
	Ptr<Packet> packet;

	WarningHeader wHeader = WarningHeader(id, originPosx, originPosy, speed);
	packet->AddHeader(wHeader);

	TypeHeader tHeader = TypeHeader(DTM_WARNING);
	packet->AddHeader(tHeader);

	return packet;
}
*/
}
}
