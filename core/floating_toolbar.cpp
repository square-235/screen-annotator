#include "floating_toolbar.h"
#include "../config/config.h"
#include "../tools/tool_factory.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QColorDialog>
#include <QPainter>
#include <QPainterPath>
#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QIcon>
#include <QPixmap>
#include <QFont>

const QMap<QString, QPair<QString, QString>> FloatingToolbar::TOOL_ICONS = {
    {"pen", {"draw-freehand", "✏️"}},
    {"line", {"draw-line", "📏"}},
    {"arrow", {"draw-arrow", "➡️"}},
    {"rectangle", {"draw-rectangle", "⬜"}},
    {"ellipse", {"draw-ellipse", "⭕"}},
    {"eraser", {"draw-eraser", "🧹"}},
    {"undo", {"edit-undo", "↩️"}},
    {"redo", {"edit-redo", "↪️"}},
    {"clear", {"edit-clear", "🗑️"}},
    {"minimize", {"window-minimize", "➖"}},
    {"exit", {"application-exit", "✕"}},
};

const QMap<QString, QString> FloatingToolbar::TOOL_LABELS = {
    {"pen", "画笔"},
    {"line", "直线"},
    {"arrow", "箭头"},
    {"rectangle", "矩形"},
    {"ellipse", "椭圆"},
    {"eraser", "橡皮"},
    {"undo", "撤销"},
    {"redo", "重做"},
    {"clear", "清除"},
    {"minimize", "最小"},
    {"exit", "退出"},
};

FloatingToolbar::FloatingToolbar(QWidget *parent)
    : QWidget(parent)
    , m_isDragging(false)
    , m_currentTool("pen")
    , m_currentColor(Config::DEFAULT_COLOR)
    , m_currentThickness(Config::DEFAULT_THICKNESS)
{
    if (parent == nullptr) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    }
    setAttribute(Qt::WA_TranslucentBackground);

    initUi();
    setupLayout();
    updateToolButtons();
}

void FloatingToolbar::initUi()
{
    setFixedSize(Config::TOOLBAR_WIDTH, Config::TOOLBAR_HEIGHT + 12);
}

QIcon FloatingToolbar::getIcon(const QString &iconName, const QString &fallbackEmoji)
{
    QIcon icon = QIcon::fromTheme(iconName + "-symbolic");
    if (icon.isNull()) {
        QPixmap pm(24, 24);
        pm.fill(Qt::transparent);
        QPainter p(&pm);
        p.setRenderHint(QPainter::Antialiasing);
        QFont font;
        font.setPointSize(16);
        p.setFont(font);
        p.setPen(QColor("#333"));
        p.drawText(pm.rect(), Qt::AlignCenter, fallbackEmoji);
        p.end();
        icon = QIcon(pm);
    }
    return icon;
}

QToolButton* FloatingToolbar::createToolButton(const QString &name, const QString &iconName,
                                                const QString &fallbackEmoji, bool checkable)
{
    QToolButton *btn = new QToolButton(this);
    btn->setObjectName(name);
    btn->setCheckable(checkable);

    QIcon icon = getIcon(iconName, fallbackEmoji);
    btn->setIcon(icon);
    btn->setIconSize(QSize(20, 20));
    btn->setText(TOOL_LABELS.value(name, ""));
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    btn->setFixedSize(60, 40);
    btn->setStyleSheet(R"(
        QToolButton {
            font-size: 10px;
            border: 1px solid rgba(200, 200, 200, 200);
            border-radius: 6px;
            background-color: rgba(245, 245, 245, 240);
            color: #333;
            padding: 2px 4px;
        }
        QToolButton:hover {
            background-color: rgba(230, 230, 230, 255);
        }
        QToolButton:checked {
            background-color: rgba(0, 120, 215, 255);
            color: white;
            border-color: rgba(0, 120, 215, 255);
        }
        QToolButton:pressed {
            background-color: rgba(0, 100, 195, 255);
        }
        QToolButton:disabled {
            color: rgba(150, 150, 150, 200);
            background-color: rgba(230, 230, 230, 200);
        }
    )");
    return btn;
}

void FloatingToolbar::addSeparator(QHBoxLayout *layout)
{
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::VLine);
    separator->setStyleSheet("color: rgba(180, 180, 180, 200);");
    layout->addWidget(separator);
}

void FloatingToolbar::setupLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QLabel *dragHandle = new QLabel("⋯⋯⋯", this);
    dragHandle->setFixedHeight(14);
    dragHandle->setAlignment(Qt::AlignCenter);
    dragHandle->setStyleSheet("color: rgba(150,150,150,200); font-size: 10px;");
    mainLayout->addWidget(dragHandle);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(10, 0, 10, 6);
    contentLayout->setSpacing(6);

    QStringList tools = {"pen", "line", "arrow", "rectangle", "ellipse", "eraser"};
    for (const QString &toolId : tools) {
        QPair<QString, QString> iconPair = TOOL_ICONS[toolId];
        QToolButton *btn = createToolButton(toolId, iconPair.first, iconPair.second, true);
        connect(btn, &QToolButton::clicked, this, [this, toolId]() {
            onToolClicked(toolId);
        });
        m_toolButtons[toolId] = btn;
        contentLayout->addWidget(btn);
    }

    addSeparator(contentLayout);

    QVBoxLayout *colorLayout = new QVBoxLayout();
    colorLayout->setSpacing(2);
    QLabel *colorLabel = new QLabel("颜色", this);
    colorLabel->setAlignment(Qt::AlignCenter);
    colorLabel->setStyleSheet("font-size: 9px; color: #555;");
    m_colorBtn = new QPushButton(this);
    m_colorBtn->setFixedSize(44, 28);
    m_colorBtn->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    border: 2px solid #888;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    border-color: #0078d7;"
        "}"
    ).arg(m_currentColor.name()));
    connect(m_colorBtn, &QPushButton::clicked, this, &FloatingToolbar::onColorClicked);
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(m_colorBtn);
    contentLayout->addLayout(colorLayout);

    QVBoxLayout *thicknessLayout = new QVBoxLayout();
    thicknessLayout->setSpacing(2);
    QLabel *thicknessLabel = new QLabel("粗细", this);
    thicknessLabel->setAlignment(Qt::AlignCenter);
    thicknessLabel->setStyleSheet("font-size: 9px; color: #555;");
    m_thicknessSlider = new QSlider(Qt::Horizontal, this);
    m_thicknessSlider->setMinimum(Config::MIN_THICKNESS);
    m_thicknessSlider->setMaximum(Config::MAX_THICKNESS);
    m_thicknessSlider->setValue(m_currentThickness);
    m_thicknessSlider->setFixedWidth(70);
    connect(m_thicknessSlider, &QSlider::valueChanged, this, &FloatingToolbar::onThicknessChanged);
    m_thicknessValueLabel = new QLabel(QString("%1px").arg(m_currentThickness), this);
    m_thicknessValueLabel->setAlignment(Qt::AlignCenter);
    m_thicknessValueLabel->setStyleSheet("font-size: 9px; color: #555;");
    thicknessLayout->addWidget(thicknessLabel);
    thicknessLayout->addWidget(m_thicknessSlider);
    thicknessLayout->addWidget(m_thicknessValueLabel);
    contentLayout->addLayout(thicknessLayout);

    addSeparator(contentLayout);

    QPair<QString, QString> undoIconPair = TOOL_ICONS["undo"];
    m_undoBtn = createToolButton("undo", undoIconPair.first, undoIconPair.second);
    connect(m_undoBtn, &QToolButton::clicked, this, &FloatingToolbar::undoRequested);
    contentLayout->addWidget(m_undoBtn);

    QPair<QString, QString> redoIconPair = TOOL_ICONS["redo"];
    m_redoBtn = createToolButton("redo", redoIconPair.first, redoIconPair.second);
    connect(m_redoBtn, &QToolButton::clicked, this, &FloatingToolbar::redoRequested);
    contentLayout->addWidget(m_redoBtn);

    QPair<QString, QString> clearIconPair = TOOL_ICONS["clear"];
    m_clearBtn = createToolButton("clear", clearIconPair.first, clearIconPair.second);
    connect(m_clearBtn, &QToolButton::clicked, this, &FloatingToolbar::clearRequested);
    contentLayout->addWidget(m_clearBtn);

    addSeparator(contentLayout);

    QPair<QString, QString> minimizeIconPair = TOOL_ICONS["minimize"];
    m_minimizeBtn = createToolButton("minimize", minimizeIconPair.first, minimizeIconPair.second);
    connect(m_minimizeBtn, &QToolButton::clicked, this, &FloatingToolbar::minimizeRequested);
    contentLayout->addWidget(m_minimizeBtn);

    QPair<QString, QString> exitIconPair = TOOL_ICONS["exit"];
    m_exitBtn = createToolButton("exit", exitIconPair.first, exitIconPair.second);
    connect(m_exitBtn, &QToolButton::clicked, this, &FloatingToolbar::exitRequested);
    contentLayout->addWidget(m_exitBtn);

    contentLayout->addStretch();
    mainLayout->addLayout(contentLayout);
}

void FloatingToolbar::onToolClicked(const QString &toolId)
{
    m_currentTool = toolId;
    updateToolButtons();
    emit toolChanged(toolId);
}

void FloatingToolbar::updateToolButtons()
{
    for (auto it = m_toolButtons.begin(); it != m_toolButtons.end(); ++it) {
        it.value()->setChecked(it.key() == m_currentTool);
    }
}

void FloatingToolbar::onColorClicked()
{
    QColor color = QColorDialog::getColor(m_currentColor, this, "选择颜色");
    if (color.isValid()) {
        setColor(color);
    }
}

void FloatingToolbar::onThicknessChanged(int value)
{
    m_currentThickness = value;
    m_thicknessValueLabel->setText(QString("%1px").arg(value));
    emit thicknessChanged(value);
}

void FloatingToolbar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (event->position().y() < 16) {
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            m_isDragging = true;
            event->accept();
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

void FloatingToolbar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void FloatingToolbar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isDragging) {
        m_isDragging = false;
        checkSnapToEdge();
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void FloatingToolbar::checkSnapToEdge()
{
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    QRect geom = frameGeometry();
    int snapDist = Config::TOOLBAR_SNAP_DISTANCE;
    int margin = Config::TOOLBAR_MARGIN;

    if (geom.left() - screen.left() < snapDist) {
        geom.moveLeft(screen.left() + margin);
    } else if (screen.right() - geom.right() < snapDist) {
        geom.moveRight(screen.right() - margin);
    }

    if (geom.top() - screen.top() < snapDist) {
        geom.moveTop(screen.top() + margin);
    } else if (screen.bottom() - geom.bottom() < snapDist) {
        geom.moveBottom(screen.bottom() - margin);
    }

    move(geom.topLeft());
}

void FloatingToolbar::setColor(const QColor &color)
{
    m_currentColor = color;
    m_colorBtn->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    border: 2px solid #888;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    border-color: #0078d7;"
        "}"
    ).arg(m_currentColor.name()));
    emit colorChanged(color);
}

void FloatingToolbar::setThickness(int thickness)
{
    m_currentThickness = thickness;
    m_thicknessSlider->blockSignals(true);
    m_thicknessSlider->setValue(thickness);
    m_thicknessSlider->blockSignals(false);
    m_thicknessValueLabel->setText(QString("%1px").arg(thickness));
}

void FloatingToolbar::setUndoEnabled(bool enabled)
{
    m_undoBtn->setEnabled(enabled);
}

void FloatingToolbar::setRedoEnabled(bool enabled)
{
    m_redoBtn->setEnabled(enabled);
}

void FloatingToolbar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    QRectF adjustedRect = rect().adjusted(0, 12, 0, 0);
    path.addRoundedRect(adjustedRect, 12, 12);
    painter.fillPath(path, QColor(245, 245, 245, 240));
    painter.end();
}

void FloatingToolbar::closeEvent(QCloseEvent *event)
{
    emit exitRequested();
    QWidget::closeEvent(event);
}
