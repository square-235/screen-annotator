#include "screen_annotator.h"
#include <QApplication>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QPen>

ScreenAnnotator::ScreenAnnotator(QObject *parent)
    : QObject(parent)
    , m_overlay(nullptr)
    , m_trayIcon(nullptr)
    , m_trayMenu(nullptr)
    , m_showAction(nullptr)
    , m_quitAction(nullptr)
{
    qApp->setQuitOnLastWindowClosed(false);

    createTrayIcon();

    m_overlay = new AnnotationOverlay();
    m_overlay->show();
}

void ScreenAnnotator::createTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(createTrayIconPixmap());

    m_trayMenu = new QMenu();

    m_showAction = new QAction("显示批注", this);
    connect(m_showAction, &QAction::triggered, this, &ScreenAnnotator::onShowFromTray);
    m_trayMenu->addAction(m_showAction);

    m_quitAction = new QAction("退出", this);
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);
    m_trayMenu->addAction(m_quitAction);

    m_trayIcon->setContextMenu(m_trayMenu);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &ScreenAnnotator::onTrayActivated);
    m_trayIcon->show();
}

QIcon ScreenAnnotator::createTrayIconPixmap()
{
    QPixmap pm(24, 24);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 120, 215));
    p.drawRoundedRect(2, 2, 20, 20, 4, 4);

    p.setPen(QPen(QColor(255, 255, 255), 2));
    p.setBrush(Qt::NoBrush);
    p.drawLine(6, 18, 14, 10);
    p.drawLine(14, 10, 18, 6);
    p.drawLine(6, 14, 10, 18);

    p.end();
    return QIcon(pm);
}

void ScreenAnnotator::onShowFromTray()
{
    if (m_overlay) {
        m_overlay->restoreFromTray();
    }
}

void ScreenAnnotator::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        onShowFromTray();
    }
}

int ScreenAnnotator::run()
{
    return qApp->exec();
}
