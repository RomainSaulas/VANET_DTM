/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "dtm-packet.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("DtmPacket");

namespace ns3 {
namespace dtm {

NS_OBJECT_ENSURE_REGISTERED (TypeHeader);

/*TypeHeader::TypeHeader ()
{
	m_valid = true;
}*/

TypeHeader::TypeHeader (MessageType t = DTM_HELLO)
	: m_type (t),
		m_valid (true)
{
}

TypeId 
TypeHeader::GetTypeId()
{
	static TypeId tid = TypeId ("ns3::dtm::TypeHeader")
		.SetParent<Header> ()
		.AddConstructor<TypeHeader> ()
	;
	return tid;
}

TypeId 
TypeHeader::GetInstanceTypeId () const
{
	return GetTypeId ();
}

uint32_t 
TypeHeader::GetSerializedSize () const
{
	return 1;
}

void 
TypeHeader::Serialize (Buffer::Iterator start) const
{
	start.WriteU8 ((uint8_t) m_type);
}

uint32_t 
TypeHeader::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;
	uint8_t type = i.ReadU8 ();
	m_valid = true;
	switch (type)
		{
		case DTM_HELLO:
		case DTM_WARNING:
			{
				m_type = (MessageType) type;
				break;
			}
		default:
			m_valid = false;		
		}
	uint32_t dist = i.GetDistanceFrom (start);
	NS_ASSERT (dist == GetSerializedSize ());
	return dist;
}

void 
TypeHeader::Print(std::ostream &os) const
{
	switch (m_type)
		{
		case DTM_HELLO:
			{
				os << "HELLO";
				break;
			}
		case DTM_WARNING:
			{
				os << "POSITION";
				break;
			}
		default:
			os << "UNKNOWN_TYPE";
		}
}

bool 
TypeHeader::operator== (TypeHeader const & o) const 
{
	return (m_type == o.m_type && m_valid == o.m_valid);
}

std::ostream & 
operator<< (std::ostream & os, TypeHeader const & h){
	h.Print (os);
	return os;
}


//-----------------------------------------------------------------------------
// HELLO
//-----------------------------------------------------------------------------
HelloHeader::HelloHeader (uint32_t id, uint64_t originPosx, uint64_t originPosy, uint64_t speedx, uint64_t speedy)
  : m_id (id),
  	m_originPosx (originPosx),
    m_originPosy (originPosy),
    m_speedx (speedx),
    m_speedy (speedy)
{
}

NS_OBJECT_ENSURE_REGISTERED (HelloHeader);

TypeId
HelloHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dtm::HelloHeader")
    .SetParent<Header> ()
    .AddConstructor<HelloHeader> ()
  ;
  return tid;
}

TypeId
HelloHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
HelloHeader::GetSerializedSize () const
{
  return 36;
}

void
HelloHeader::Serialize (Buffer::Iterator i) const
{
  NS_LOG_DEBUG ("Serialize Id " << m_id 
  							<< " X " << m_originPosx 
   							<< " Y " << m_originPosy 
   							<< " Speed X " << m_speedx
   							<< " Speed Y " << m_speedy);

  i.WriteHtonU32 (m_id);
  i.WriteHtonU64 (m_originPosx);
  i.WriteHtonU64 (m_originPosy);
  i.WriteHtonU64 (m_speedx);
  i.WriteHtonU64 (m_speedy);
}

uint32_t
HelloHeader::Deserialize (Buffer::Iterator start)
{

				  Buffer::Iterator i = start;

				  m_id = i.ReadNtohU32 ();
				  m_originPosx = i.ReadNtohU64 ();
				  m_originPosy = i.ReadNtohU64 ();
				  m_speedx = i.ReadNtohU64 ();
				  m_speedy = i.ReadNtohU64 ();


				  NS_LOG_DEBUG ("Deserialize Id " << m_id 
				  							<< " X " << m_originPosx 
				  							<< " Y " << m_originPosy
				  							<< " Speed X " << m_speedx
				  							<< " Speed Y " << m_speedy);

				  uint32_t dist = i.GetDistanceFrom (start);
				  NS_ASSERT (dist == GetSerializedSize ());
				  return dist;
}

void
HelloHeader::Print (std::ostream &os) const
{
  os << " Id " << m_id 
		 << " X " << m_originPosx 
		 << " Y " << m_originPosy 
		 << " Speed X " << m_speedx
 		 << " Speed Y " << m_speedy;
}

std::ostream &
operator<< (std::ostream & os, HelloHeader const & h)
{
  h.Print (os);
  return os;
}



bool
HelloHeader::operator== (HelloHeader const & o) const
{
  return (m_id == o.m_id &&
  				m_originPosx == o.m_originPosx &&
  				m_originPosy == o.m_originPosy &&
  				m_speedx == o.m_speedx &&
  				m_speedy == o.m_speedy);
}
//-----------------------------------------------------------------------------
// WARNING
//-----------------------------------------------------------------------------static TypeId 

WarningHeader::WarningHeader (uint32_t sourceId, uint32_t prevHopId, uint32_t hopCount, uint32_t messageId, uint64_t postionx, uint64_t positiony) 
	: m_sourceId (sourceId),
		m_prevHopId (prevHopId),
		m_hopCount (hopCount),
		m_messageId (messageId),
		m_positionx (postionx),
		m_positiony (positiony)
{
}

NS_OBJECT_ENSURE_REGISTERED (WarningHeader);

TypeId
WarningHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dtm::WarningHeader")
    .SetParent<Header> ()
    .AddConstructor<WarningHeader> ()
  ;
  return tid;
}

TypeId 
WarningHeader::GetInstanceTypeId () const
{
	  return GetTypeId ();
}

uint32_t 
WarningHeader::GetSerializedSize () const
{
	return 32;
}

void 
WarningHeader::Serialize (Buffer::Iterator start) const
{
	NS_LOG_DEBUG ("Serialize Id " << m_sourceId << " MessageId " << m_messageId);

  start.WriteHtonU32 (m_sourceId);
  start.WriteHtonU32 (m_prevHopId);
  start.WriteHtonU32 (m_hopCount);
  start.WriteHtonU32 (m_messageId);
  start.WriteHtonU64 (m_positionx);
  start.WriteHtonU64 (m_positiony);
}

uint32_t 
WarningHeader::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

  m_sourceId = i.ReadNtohU32 ();
  m_prevHopId = i.ReadNtohU32 ();
	m_hopCount = i.ReadNtohU32 ();
  m_messageId = i.ReadNtohU32 ();
  m_positionx = i.ReadNtohU64 ();
  m_positiony = i.ReadNtohU64 ();

  NS_LOG_DEBUG ("Deserialize Id " << m_sourceId << " MessageId " << m_messageId);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void 
WarningHeader::Print (std::ostream &os) const
{
	 os << " Id " << m_sourceId << " MessageId " << m_messageId;
}


}
}