/*
 * $Id$
 *
 *  Created on: Dec 13, 2008
 *      Author: koheik
 */
#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QMutexLocker>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "AnRoot.h"
#include "AnMaster.h"
#include "AnAddress.h"
#include "AnThub.h"
#include "AnSerdes.h"
#include "AnTcpu.h"
#include "AnTdig.h"
#include "AnTdc.h"

AnMaster::AnMaster(AnRoot *root, QObject *parent)
 : QThread(parent)
{
	m_root = root;
}

AnMaster::~AnMaster()
{
	// d-tor
}

void AnMaster::setMode(int mode)
{
	m_mutex.lock();
	m_q.enqueue(mode);
	m_mutex.unlock();
	start();
}

void AnMaster::run()
{
	static int count = 0;

	m_root->log(QString("AnMaster::run: begin (%1)").arg(count));
	m_mutex.lock();
	while(m_q.count() > 0) p_setMode(m_q.dequeue());
	m_mutex.unlock();
	m_root->log(QString("AnMaster::run: finish (%1)").arg(count));
	++count;
}

void AnMaster::p_setMode(int mode)
{

  QSqlQuery qry;
  qry.prepare("SELECT c.id, ct.name, c.addr1, c.addr2, c.addr3, c.addr4, c.val"
	      " FROM configs c"
	      " INNER JOIN config_types ct ON c.config_type_id = ct.id"
	      " WHERE c.config_set_id=:cs_id"
	      " ORDER BY c.config_set_order");
  qry.bindValue(":cs_id", mode);
  qry.exec();
  while (qry.next()) {
    int id        = qry.value(0).toInt();
    QString ct    = qry.value(1).toString();
    int addr1     = qry.value(2).toInt();
    int addr2     = qry.value(3).toInt();
    int addr3     = qry.value(4).toInt();
    int addr4     = qry.value(5).toInt();
    int val       = qry.value(6).toInt();
    AnAddress addr(addr1, addr2, addr3, addr4);
    
    m_root->log(QString("AnMaster: %1 %2 %3").arg(id).arg(ct).arg(val));
    // THUB
    if (ct == "THUB_ENABLE") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnThub *thub = dynamic_cast<AnThub*>( m_root->find(ad) );
	if (thub) thub->setActive(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "THUB_TEMP_ALARM") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnThub *thub = dynamic_cast<AnThub*>( m_root->find(ad) );
	if (thub) thub->setTempAlarm(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    
    // Serdes
    else if (ct == "SRDS_ENABLE") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnSerdes *srds = dynamic_cast<AnSerdes*>( m_root->find(ad) );
	if (srds) srds->setActive(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "SRDS_PLDREG9XBASE") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnSerdes *srds = dynamic_cast<AnSerdes*>( m_root->find(ad) );
	if (srds) srds->setPld9xBase(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    
    // TCPU
    else if (ct == "TCPU_ENABLE") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) tcpu->setActive(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TCPU_TEMP_ALARM") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) tcpu->setTempAlarm(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TCPU_PLDREG02") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) tcpu->setPldReg02Set(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TCPU_PLDREG03") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) tcpu->setPldReg03Set(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TCPU_PLDREG0E") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) tcpu->setPldReg0eSet(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TCPU_EEPROM") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) tcpu->setEeprom(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TCPU_MULTGATEPHASE") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) tcpu->setMultGatePhase(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    
    // TDIG
    else if (ct == "TDIG_ENABLE") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTdig *tdig = dynamic_cast<AnTdig*>( m_root->find(ad) );
	if (tdig) tdig->setActive(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TDIG_TEMP_ALARM") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTdig *tdig = dynamic_cast<AnTdig*>( m_root->find(ad) );
	if (tdig) tdig->setTempAlarm(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TDIG_THRESHOLD") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTdig *tdig = dynamic_cast<AnTdig*>( m_root->find(ad) );
	if (tdig) tdig->setThreshold(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TDIG_EEPROM") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTdig *tdig = dynamic_cast<AnTdig*>( m_root->find(ad) );
	if (tdig) tdig->setEeprom(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    
    // TDC
    else if (ct == "TDC_ENABLE"){
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTdc *tdc = dynamic_cast<AnTdc*>( m_root->find(ad) );
	if (tdc) tdc->setActive(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TDC_CONFIG_ID"){
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTdc *tdc = dynamic_cast<AnTdc*>( m_root->find(ad) );
	if (tdc) tdc->setConfigId(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    
    else if (ct == "TDC_CH_MASK"){
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTdc *tdc = dynamic_cast<AnTdc*>( m_root->find(ad) );
	if (tdc) tdc->setMask(val);
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    
    // Action Items
    else if (ct == "INIT") {
      m_root->init(val, m_root->find( m_root->expand(addr) ) );
      m_root->wait();
    }
    else if (ct == "CONFIG") {
      m_root->config(val, m_root->find( m_root->expand(addr) ) );
      m_root->wait();
    }
    else if (ct == "RESET") {
      m_root->reset(val, m_root->find( m_root->expand(addr) ) );
      m_root->wait();
    }
    else if (ct == "QRESET") {
      m_root->qreset(val, m_root->find( m_root->expand(addr) ) );
      m_root->wait();
    }
    else if (ct == "SYNC") {
      m_root->sync(val,  m_root->find( m_root->expand(addr) ) );
      m_root->wait();
    }
    else if (ct == "USLEEP") {
      usleep(val);
    }
    else if (ct == "AUTOSYNC") {
      // turn on and off the auto sync action
      if (val == 1) {
	m_root->doingRecovery(false);
      }
      else {
	m_root->doingRecovery(true);
      }
    }

    else if (ct == "THUB_BUNCH_RESET") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnThub *thub = dynamic_cast<AnThub*>( m_root->find(ad) );
	if (thub) {
	  m_root->disableWatch(thub->agent()->id());
	  thub->bunchReset(val);
	  m_root->enableWatch(thub->agent()->id());
	}
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "THUB_SEND_ALERT") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnThub *thub = dynamic_cast<AnThub*>( m_root->find(ad) );
	if (thub) {
	  m_root->disableWatch(thub->agent()->id());
	  thub->sendAlert(val);
	  m_root->enableWatch(thub->agent()->id());
	}
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "THUB_RECOVERY_MSG") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnThub *thub = dynamic_cast<AnThub*>( m_root->find(ad) );
	if (thub) {
	  m_root->disableWatch(thub->agent()->id());
	  thub->recoverAlertMsg(val);
	  m_root->enableWatch(thub->agent()->id());
	}
	else qDebug() << "invalid address: " << ad.toString();
      }
    }
    else if (ct == "TCPU_RELINK") {
      foreach(AnAddress ad, m_root->expand(addr)) {
	AnTcpu *tcpu = dynamic_cast<AnTcpu*>( m_root->find(ad) );
	if (tcpu) {
	  m_root->disableWatch(tcpu->agent()->id());
	  tcpu->relink(val);
	  m_root->enableWatch(tcpu->agent()->id());
	} else qDebug() << "invalid address: " << ad.toString();
      }
    }

    // Resets for TCPU and THUB
    else if (ct == "TCPU_RECOVERY") {
      m_root->tcpurecovery(val, m_root->find( m_root->expand(addr) ) );
      m_root->wait();
    }
    else if (ct == "THUB_RECOVERY") {
      m_root->thubrecovery(val, m_root->find( m_root->expand(addr) ) );
      m_root->wait();
    }
  }
  m_root->emit_finished();
  m_root->emit_updated();
}
