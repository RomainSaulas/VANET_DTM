/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/dtm.h"
#include "ns3/dtm-packet.h"
#include "ns3/dtm-wqueue.h"
#include "ns3/dtm-ptable.h"


// An essential include is test.h
#include "ns3/test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
namespace ns3 
{
namespace dtm 
{

// This is an example TestCase.
class DtmTestCase1 : public TestCase
{
public:
  DtmTestCase1 ();
  virtual ~DtmTestCase1 ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
DtmTestCase1::DtmTestCase1 ()
  : TestCase ("Dtm test case (does nothing)")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
DtmTestCase1::~DtmTestCase1 ()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
DtmTestCase1::DoRun (void)
{
  // A wide variety of test macros are available in src/core/test.h
  NS_TEST_ASSERT_MSG_EQ (true, true, "true doesn't equal true for some reason");
  // Use this one for floating point comparisons
  NS_TEST_ASSERT_MSG_EQ_TOL (0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}
//-----------------------------------------------------------------------------
struct TypeHeaderTest : public TestCase
{
  TypeHeaderTest () : TestCase ("AODV TypeHeader") 
  {
  }
  virtual void DoRun ()
  {
    TypeHeader h (DTM_HELLO);
    NS_TEST_EXPECT_MSG_EQ (h.IsValid (), true, "Default header is valid");
    NS_TEST_EXPECT_MSG_EQ (h.Get (), DTM_HELLO, "Default header is RREQ");

    Ptr<Packet> p = Create<Packet> ();
    p->AddHeader (h);
    TypeHeader h2 (DTM_WARNING);
    uint32_t bytes = p->RemoveHeader (h2);
    NS_TEST_EXPECT_MSG_EQ (bytes, 1, "Type header is 1 byte long");
    NS_TEST_EXPECT_MSG_EQ (h, h2, "Round trip serialization works");
  }
};
// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class DtmTestSuite : public TestSuite
{
public:
  DtmTestSuite ();
};

DtmTestSuite::DtmTestSuite ()
  : TestSuite ("dtm", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new DtmTestCase1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static DtmTestSuite dtmTestSuite;

  
}
}
