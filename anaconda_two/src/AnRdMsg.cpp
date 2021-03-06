/*
 * $Id$
 *
 *  Created on: Dec 10, 2008
 *      Author: koheik
 */

#include "AnRdMsg.h"

AnRdMsg::AnRdMsg()
{
  // c-tor
}

// AnRdMsg::AnRdMsg(const quint8 devid, const PCAN::TPCANRdMsg &rmsg)
// {
//   m_devid = devid;
//   m_id    = rmsg.Msg.ID;
//   m_type  = rmsg.Msg.MSGTYPE;
//   m_len   = rmsg.Msg.LEN;
//   for (int i = 0; i < m_len; ++i) m_data[i] = rmsg.Msg.DATA[i];
// }

AnRdMsg::AnRdMsg(const quint8 devid, const struct can_frame &msg)
{
  m_devid = devid;
  m_id    = msg.can_id;
  m_type  = (msg.can_id & MSGTYPE_EXTENDED) ? 1 : 0;
  m_len   = msg.can_dlc;
  for (int i = 0; i < m_len; ++i) m_data[i] = msg.data[i];
}

AnRdMsg::~AnRdMsg()
{
  // d-tor
}

quint64 AnRdMsg::data() const
{
  quint64 ret = 0;
  for(int i = 0; i < m_len; ++i) ret = (ret << 8) | m_data[i];
  return ret;
}

AnAddress AnRdMsg::source() const
{
  quint8 tcpu;
  quint8 tdig;

  if ((m_id & MSGTYPE_EXTENDED) == MSGTYPE_EXTENDED) {
    tcpu = (m_id & 0xff);
    tdig = (m_id >> 22) & 0xff;
  } else {
    tcpu = (m_id >> 4) & 0xff;
    tdig = 0;
  }
  return AnAddress(m_devid, tcpu, tdig, 0);
}

quint8 AnRdMsg::payload() const
{
  if ((m_id & MSGTYPE_EXTENDED) == MSGTYPE_EXTENDED)
    return ((m_id >> 18) & 0xf);
  else
    return (m_id & 0xf);
}

QString AnRdMsg::toString() const
{
  char buf1[128];
  char buf2[16];
  int length;
  
  sprintf(buf1, "AnRdMsg(DEVID=%d ID=0x%x TYPE=0x%x LEN=%d DATA=[",
	  devid(), id(), type(), len());
  if (len() < 9)
    length = len();
  else
    length = 8;

  for(int i = 0; i < length; ++i) {
    if (i == 0)
      sprintf(buf2, "0x%02x", data(i));
    else
      sprintf(buf2, " 0x%02x", data(i));
    strcat(buf1, buf2);
  }
  strcat(buf1, "])");
  return QString(buf1);
}

QDebug operator<<(QDebug dbg, const AnRdMsg &m)
{
  char buf1[128];
  char buf2[16];
  int length;

  sprintf(buf1, "AnRdMsg(DEVID=%d ID=0x%x TYPE=0x%x LEN=%d DATA=[",
	  m.devid(), m.id(), m.type(), m.len());
  
  if (m.len() < 9)
    length = m.len();
  else
    length = 8;

  for(int i = 0; i < length; ++i) {
    if (i == 0)
      sprintf(buf2, "0x%02x", m.data(i));
    else
      sprintf(buf2, " 0x%02x", m.data(i));
    strcat(buf1, buf2);
  }
  strcat(buf1, "])");
  dbg << buf1;
  return dbg;
}
