#ifndef SCREEN_ANNOTATOR_H
#define SCREEN_ANNOTATOR_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <memory>

#include "core/annotation_overlay.h"

class ScreenAnnotator : public QObject {
    Q_OBJECT
public:
    explicit ScreenAnnotator(QObject *parent = nullptr);
    ~ScreenAnnotator() override = default;

    int run();

private slots:
    void onShowFromTray();
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    AnnotationOverlay *m_overlay;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    QAction *m_showAction;
    QAction *m_quitAction;

    void createTrayIcon();
    QIcon createTrayIconPixmap();
};

#endif
