/*
 * $Id$
 *
 *  Created on: Nov 11, 2008
 *      Author: koheik
 */

#ifndef KTDIGVIEW_H_
#define KTDIGVIEW_H_
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>

#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>

#include <QtGui/QTableView>
#include <QtGui/QStandardItemModel>
#include <QtGui/QItemSelectionModel>

#include "AnTdig.h"

class KTdigView : public QGroupBox {
	Q_OBJECT
public:
	KTdigView(QWidget *parent = 0);
	virtual ~KTdigView();

	QItemSelectionModel *selectionModel() const { return m_selectionModel; }
	void setSelectionModel(QItemSelectionModel *md);	

public slots:
	void currentRowChanged(const QModelIndex &current, const QModelIndex &parent);
	void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);


private:
	QItemSelectionModel *m_selectionModel;

	QGroupBox *m_box;
	QLabel    *m_laddr, *l_laddr;
	QLabel    *m_haddr, *l_haddr;
	QLabel    *m_firm,  *l_firm;
	QLabel    *m_chip,  *l_chip;
	QLabel    *m_temp,  *l_temp;
	QLabel    *m_ecsr,  *l_ecsr;
	QLabel    *m_pld03, *l_pld03;
	QLabel    *m_thrs,  *l_thrs;
	QLabel    *m_status[3], *l_status[3];
	QLabel    *m_eeprom, *l_eeprom;
};

#endif /* KTCPUVIEW_H_ */
