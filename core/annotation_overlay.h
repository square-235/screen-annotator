#ifndef ANNOTATION_OVERLAY_H
#define ANNOTATION_OVERLAY_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QColor>
#include <QString>
#include <memory>
#include <QTouchEvent>

#include "../tools/base_tool.h"
#include "history_manager.h"
#include "floating_toolbar.h"

class AnnotationOverlay : public QWidget {
    Q_OBJECT
public:
    explicit AnnotationOverlay(QWidget *parent = nullptr);

    bool canUndo() const;
    bool canRedo() const;

public slots:
    void setTool(const QString &toolName);
    void setColor(const QColor &color);
    void setThickness(int thickness);
    void undo();
    void redo();
    void clearAll();
    void minimizeToTray();
    void restoreFromTray();

signals:
    void exitRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    bool event(QEvent *event) override;

private slots:
    void onHistoryChanged();
    void onExitApp();

private:
    QString m_currentToolName;
    QColor m_currentColor;
    int m_currentThickness;
    BaseToolPtr m_currentTool;

    HistoryManager *m_history;
    FloatingToolbar *m_toolbar;

    QPixmap m_cachedPixmap;
    bool m_cacheDirty;
    bool m_toolbarParent;
    QPoint m_savedToolbarPos;

    void initWindow();
    void createTool();
    void createToolbar();
    void rebuildCache();
    bool isOnToolbar(const QPoint &pos) const;
    void updateUndoRedoState();
    void handleEraserResult();
};

#endif
