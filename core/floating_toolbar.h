#ifndef FLOATING_TOOLBAR_H
#define FLOATING_TOOLBAR_H

#include <QWidget>
#include <QToolButton>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QPoint>
#include <QColor>
#include <QMap>

class FloatingToolbar : public QWidget {
    Q_OBJECT
public:
    explicit FloatingToolbar(QWidget *parent = nullptr);

    void setColor(const QColor &color);
    void setThickness(int thickness);
    void setUndoEnabled(bool enabled);
    void setRedoEnabled(bool enabled);

signals:
    void toolChanged(const QString &toolName);
    void colorChanged(const QColor &color);
    void thicknessChanged(int thickness);
    void undoRequested();
    void redoRequested();
    void clearRequested();
    void minimizeRequested();
    void exitRequested();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onToolClicked(const QString &toolId);
    void onColorClicked();
    void onThicknessChanged(int value);

private:
    QPoint m_dragPosition;
    bool m_isDragging;
    QString m_currentTool;
    QColor m_currentColor;
    int m_currentThickness;

    QMap<QString, QToolButton*> m_toolButtons;
    QPushButton *m_colorBtn;
    QSlider *m_thicknessSlider;
    QLabel *m_thicknessValueLabel;
    QToolButton *m_undoBtn;
    QToolButton *m_redoBtn;
    QToolButton *m_clearBtn;
    QToolButton *m_minimizeBtn;
    QToolButton *m_exitBtn;

    void initUi();
    void setupLayout();
    void updateToolButtons();
    QToolButton* createToolButton(const QString &name, const QString &iconName,
                                  const QString &fallbackEmoji, bool checkable = false);
    QIcon getIcon(const QString &iconName, const QString &fallbackEmoji);
    void addSeparator(class QHBoxLayout *layout);
    void checkSnapToEdge();

    static const QMap<QString, QPair<QString, QString>> TOOL_ICONS;
    static const QMap<QString, QString> TOOL_LABELS;
};

#endif
