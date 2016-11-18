/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 NITK Surathkal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Priya S Tavarmani <priyast663@gmail.com>
 *          Viyom Mittal <viyommittal@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

class PiQueueDiscTestItem : public QueueDiscItem {
public:
  PiQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol);
  virtual ~PiQueueDiscTestItem ();
  virtual void AddHeader (void);

private:
  PiQueueDiscTestItem ();
  PiQueueDiscTestItem (const PiQueueDiscTestItem &);
  PiQueueDiscTestItem &operator = (const PiQueueDiscTestItem &);
};

PiQueueDiscTestItem::PiQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol)
  : QueueDiscItem (p, addr, protocol)
{
}

PiQueueDiscTestItem::~PiQueueDiscTestItem ()
{
}

void
PiQueueDiscTestItem::AddHeader (void)
{
}

class PiQueueDiscTestCase : public TestCase
{
public:
  PiQueueDiscTestCase ();
  virtual void DoRun (void);
private:
  void Enqueue (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt);
  void EnqueueWithDelay (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt);
  void RunPiTest (StringValue mode);
};

PiQueueDiscTestCase::PiQueueDiscTestCase ()
  : TestCase ("Sanity check on the PI queue disc implementation")
{
}

void
PiQueueDiscTestCase::RunPiTest (StringValue mode)
{
  uint32_t pktSize = 0;
  // 1 for packets; pktSize for bytes
  uint32_t modeSize = 1;
  uint32_t qSize = 8;
  Ptr<PiQueueDisc> queue = CreateObject<PiQueueDisc> ();

  // test 1: simple enqueue/dequeue with no drops
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", DoubleValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("A", DoubleValue (0.125)), true,
                         "Verify that we can actually set the attribute A");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("B", DoubleValue (1.25)), true,
                         "Verify that we can actually set the attribute B");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MeanPktSize", UintegerValue (1000)), true,
                         "Verify that we can actually set the attribute MeanPktSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueRef", DoubleValue (4)), true,
                         "Verify that we can actually set the attribute QueueRef");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("W", DoubleValue (125)), true,
                         "Verify that we can actually set the attribute W");

  Address dest;

  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      pktSize = 500;
      modeSize = pktSize;
      queue->SetQueueLimit (qSize * modeSize);
    }

  Ptr<Packet> p1, p2, p3, p4, p5, p6, p7, p8;
  p1 = Create<Packet> (pktSize);
  p2 = Create<Packet> (pktSize);
  p3 = Create<Packet> (pktSize);
  p4 = Create<Packet> (pktSize);
  p5 = Create<Packet> (pktSize);
  p6 = Create<Packet> (pktSize);
  p7 = Create<Packet> (pktSize);
  p8 = Create<Packet> (pktSize);

  queue->Initialize ();
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 0 * modeSize, "There should be no packets in there");
  queue->Enqueue (Create<PiQueueDiscTestItem> (p1, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 1 * modeSize, "There should be one packet in there");
  queue->Enqueue (Create<PiQueueDiscTestItem> (p2, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 2 * modeSize, "There should be two packets in there");
  queue->Enqueue (Create<PiQueueDiscTestItem> (p3, dest, 0));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p4, dest, 0));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p5, dest, 0));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p6, dest, 0));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p7, dest, 0));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p8, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 8 * modeSize, "There should be eight packets in there");

  Ptr<QueueDiscItem> item;

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the first packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 7 * modeSize, "There should be seven packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p1->GetUid (), "was this the first packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the second packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 6 * modeSize, "There should be six packet in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p2->GetUid (), "Was this the second packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the third packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 5 * modeSize, "There should be five packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p3->GetUid (), "Was this the third packet ?");

  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item == 0), true, "There are really no packets in there");

  struct d {
    uint32_t test2;
    uint32_t test3;
  } drop;


  // test 2: default values for PI parameters
  queue = CreateObject<PiQueueDisc> ();
  qSize = 300 * modeSize;
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MeanPktSize", UintegerValue (pktSize)), true,
                         "Verify that we can actually set the attribute MeanPktSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueRef", DoubleValue (50)), true,
                         "Verify that we can actually set the attribute QueueRef");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", DoubleValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("W", DoubleValue (170)), true,
                         "Verify that we can actually set the attribute W");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 300);
  Simulator::Stop (Seconds (40));
  Simulator::Run ();
  PiQueueDisc::Stats st = StaticCast<PiQueueDisc> (queue)->GetStats ();
  drop.test2 = st.unforcedDrop;
  NS_TEST_EXPECT_MSG_NE (drop.test2, 0, "There should be some unforced drops");


  // test 3: high value of W
  queue = CreateObject<PiQueueDisc> ();
  qSize = 300 * modeSize;
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MeanPktSize", UintegerValue (pktSize)), true,
                         "Verify that we can actually set the attribute MeanPktSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueRef", DoubleValue (50)), true,
                         "Verify that we can actually set the attribute QueueRef");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", DoubleValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("W", DoubleValue (500)), true,
                         "Verify that we can actually set the attribute W");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 300);
  Simulator::Stop (Seconds (40));
  Simulator::Run ();
  st = StaticCast<PiQueueDisc> (queue)->GetStats ();
  drop.test3 = st.unforcedDrop;
  NS_TEST_EXPECT_MSG_GT (drop.test3, drop.test2, "Test 3 should have more unforced drops than Test 2");
}

void 
PiQueueDiscTestCase::Enqueue (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<PiQueueDiscTestItem> (Create<Packet> (size), dest, 0));
    }
}

void 
PiQueueDiscTestCase::EnqueueWithDelay (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  double delay = 0.1;
  for (uint32_t i=0;i<nPkt;i++)
  {
      Simulator::Schedule (Time (Seconds ((i + 1) * delay)), &PiQueueDiscTestCase::Enqueue,this,queue,size, 1);
  }

}

void
PiQueueDiscTestCase::DoRun (void)
{
  RunPiTest (StringValue ("QUEUE_MODE_PACKETS"));
  RunPiTest (StringValue ("QUEUE_MODE_BYTES"));
  Simulator::Destroy ();
}

static class PiQueueDiscTestSuite : public TestSuite
{
public:
  PiQueueDiscTestSuite ()
    : TestSuite ("pi-queue-disc", UNIT)
  {
    AddTestCase (new PiQueueDiscTestCase (), TestCase::QUICK);
  }
} g_piQueueTestSuite;
