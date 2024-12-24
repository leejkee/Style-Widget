#include "framewgt.h"

FrameWgt::FrameWgt(QWidget *centerWidget)
{
    m_pCenter_widget = centerWidget;
    initialize();
}

FrameWgt::~FrameWgt()
{

}

void FrameWgt::setTitleBarColor(const QColor &color)
{
    m_pTitleBar->setBackgroundColor(color);
}

void FrameWgt::setTitleTextColor(const QColor &color)
{
    m_pTitleBar->setTextColor(color);
}

void FrameWgt::setTitleText(const QString &text)
{
    m_pTitleBar->setTitleText(text);
}

void FrameWgt::setTitleIcon(const QString &path)
{
    m_pTitleBar->setTitleIcon(path);
}

void FrameWgt::setTitleIcon(const QPixmap &icon)
{
    m_pTitleBar->setTitleIcon(icon);
}

void FrameWgt::setRadius(const uint &r)
{
    m_radius = r;
    updateRadius(m_radius);
}

void FrameWgt::setShadowColor(const QColor &color)
{
    m_shadowColor = color;
    updateShadow();
}

void FrameWgt::setBlurRadius(const uint &r)
{
    m_blurRadius = r;
    updateShadow();
}

void FrameWgt::setHiddenMin(const bool &is)
{
    m_pTitleBar->setHiddenMin(is);
    updateShadow();
}

void FrameWgt::setHiddenMax(const bool &is)
{
    m_pTitleBar->setHiddenMax(is);
}

void FrameWgt::setHiddenTitleBar(const bool &is)
{
    m_pTitleBar->setHidden(is);
    updateRadius(m_radius);
}

void FrameWgt::setTitleBarHeight(const uint &h)
{
    m_pTitleBar->setHeight(h);
}

void FrameWgt::setMinIcon(const QIcon &icon)
{
    m_pTitleBar->setMinIcon(icon);
}

void FrameWgt::setMaxIcon(const QIcon &icon)
{
    m_pTitleBar->setMaxIcon(icon);
}

void FrameWgt::setCloseIcon(const QIcon &icon)
{
    m_pTitleBar->setCloseIcon(icon);
}

void FrameWgt::setTitleTextFont(const QFont &font)
{
    m_pTitleBar->setTitleTextFont(font);
}

void FrameWgt::addWidgetToTitleBar(QWidget *w)
{
    m_pTitleBar->addWidget(w);
}

void FrameWgt::addLayoutToTitleBar(QLayout *layout)
{
    m_pTitleBar->addLayout(layout);
}

void FrameWgt::onMax(bool is)
{
    if(this->isHidden())
    {
        return;
    }
    if(is)
    {
        updateRadius(0);
    }
    else
    {
        updateRadius(m_radius);
    }
}


void FrameWgt::initialize()
{
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // 隐藏默认边框
    this->setAttribute(Qt::WA_TranslucentBackground, true); // 设置透明背景
    setMouseTracking(true); // 启用鼠标追踪
    this->resize(400, 300);
    m_pBorder = new QWidget;
    m_pBorder->setCursor(Qt::ArrowCursor);
    m_pTitleBar = new TitleBar(m_pBorder);
    QVBoxLayout *pCenterLayout = new QVBoxLayout;
    pCenterLayout->setContentsMargins(0, 0,0 ,0);
    pCenterLayout->setSpacing(0);
    pCenterLayout->setAlignment(Qt::AlignTop);
    pCenterLayout->addWidget(m_pTitleBar);

    if(m_pCenter_widget){
        pCenterLayout->addWidget(m_pCenter_widget);
        pCenterLayout->setStretch(1, 1);
        setTitleText(m_pCenter_widget->windowTitle());
        m_pBorder->setCursor(m_pCenter_widget->cursor());
    }
    m_pBorder->setLayout(pCenterLayout);
    m_pShadow = new QGraphicsDropShadowEffect(this);
    m_pShadow->setOffset(0,0); // 偏移量
    m_pShadow->setColor(m_shadowColor);
    m_pShadow->setBlurRadius(m_blurRadius); // 设置阴影范围
    m_pBorder->setGraphicsEffect(m_pShadow); // 应用阴影
    m_pGridLayout = new QGridLayout;
    m_pGridLayout->setContentsMargins(m_blurRadius, m_blurRadius, m_blurRadius, m_blurRadius); // 给边框阴影预留位置
    m_pGridLayout->addWidget(m_pBorder);
    this->setLayout(m_pGridLayout);

    setTitleBarColor(Qt::white);
    setRadius(m_radius);

    connect(m_pTitleBar, SIGNAL(closed()), this, SLOT(close()));
    connect(m_pTitleBar, SIGNAL(maxChange(bool)), this, SLOT(onMax(bool)));
}

void FrameWgt::calculateOpflag(QPoint pos)
{
    QCursor cursor = Qt::ArrowCursor;
    m_opFlag = NONE;
    if(pos.x() < m_blurRadius)
    {
        if(pos.y() < m_blurRadius + m_radius)
        {
            m_opFlag = OpFlag::TL;
            cursor = Qt::SizeFDiagCursor;
        }
        else if(pos.y() > this->height() - m_blurRadius - m_radius)
        {
            m_opFlag = OpFlag::BL;
            cursor = Qt::SizeBDiagCursor;
        }
        else
        {
            m_opFlag = OpFlag::Left;
            cursor = Qt::SizeHorCursor;
        }
    }
    else if(pos.x() > this->width() - m_blurRadius)
    {
        if(pos.y() < m_blurRadius + m_radius)
        {
            m_opFlag = OpFlag::TR;
            cursor = Qt::SizeBDiagCursor;
        }
        else if(pos.y() > this->height() - m_blurRadius - m_radius)
        {
            m_opFlag = OpFlag::BR;
            cursor = Qt::SizeFDiagCursor;
        }
        else
        {
            m_opFlag = OpFlag::Right;
            cursor = Qt::SizeHorCursor;
        }
    }
    else
    {
        if(pos.y() < m_blurRadius)
        {
            m_opFlag = OpFlag::Top;
            cursor = Qt::SizeVerCursor;
        }
        else if(pos.y() > this->height() - m_blurRadius)
        {
            m_opFlag = OpFlag::Bottom;
            cursor = Qt::SizeVerCursor;
        }
        else
        {
            m_opFlag = OpFlag::NONE;
            cursor = Qt::ArrowCursor;
        }
    }
    this->setCursor(cursor);
}

void FrameWgt::updateRadius(const uint &r)
{
    m_pBorder->setStyleSheet(QString("border-radius: %1px;").arg(QString::number(r)));

    m_pTitleBar->setRadius(r);
    if(m_pCenter_widget != nullptr)
    {
        QString topR = "0";
        if(m_pTitleBar->isHidden())
        {
            topR = QString::number(m_radius);
        }
        m_pCenter_widget->setStyleSheet(QString(m_pCenter_widget->styleSheet() + "border-top-left-radius: %2px;"
                                                                                 "border-top-right-radius: %2px;"
                                                                                 "border-bottom-left-radius: %1px;"
                                                                                 "border-bottom-right-radius: %1px;").arg(QString::number(r), topR));
    }
}

void FrameWgt::updateShadow()
{
    m_pShadow->setBlurRadius(m_blurRadius);
    m_pShadow->setColor(m_shadowColor);
    if(this->isMaximized() || this->isFullScreen())
    {
        m_pGridLayout->setContentsMargins(0, 0, 0, 0);
    }
    else
    {
        m_pGridLayout->setContentsMargins(m_blurRadius, m_blurRadius, m_blurRadius, m_blurRadius);
    }
}

void FrameWgt::resizeEvent(QResizeEvent *event)
{
    if(this->isHidden())
    {
        return;
    }
    if(this->isMaximized() || this->isFullScreen())
    {
        m_pGridLayout->setContentsMargins(0, 0, 0, 0);
    }
    else
    {
        m_pGridLayout->setContentsMargins(m_blurRadius, m_blurRadius, m_blurRadius, m_blurRadius);
    }
    QApplication::processEvents();
    QWidget::resizeEvent(event);
}

void FrameWgt::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(m_pBorder->geometry().contains(pos))
    {
        event->ignore();
        return;
    }
    if(m_opFlag != -1)
    {
        m_isOp = true;
        m_lastPos = event->globalPosition();
    }
}

void FrameWgt::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(!m_isOp)
    {
        calculateOpflag(pos);
    }

    //获取鼠标拖动的位置，
    QPointF currPos = event->globalPosition();
    // 计算鼠标按下并拖动的的位移
    QPointF ptemp = currPos - m_lastPos ;
    //保存当前鼠标拖动的位置，用于下一次计算拖动位移
    m_lastPos = currPos;
    event->ignore();
    if(m_isOp)
    {
        if(this->isMaximized()) return;
        int x = 0,y = 0,w = 0,h = 0;
        // 根据拖动的那一条边框，确定拉伸还是缩小窗口。
        switch (m_opFlag) {
        // 左边框被拉伸
        case Left:
            x = this->pos().x() + ptemp.x();
            y = this->pos().y();
            w = this->size().width() - ptemp.x();
            h = this->size().height();
            if(w < this->minimumWidth())
            {
                m_lastPos.setX(this->pos().x());
            }
            break;
            // 右边框被拉伸
        case Right:
            x = this->pos().x();
            y = this->pos().y();
            w = this->size().width() + ptemp.x();
            h = this->size().height();
            if(w < this->minimumWidth())
            {
                m_lastPos.setX(this->pos().x() + this->width());
            }
            break;
            // 上边框被拉伸
        case Top:
            x = this->pos().x();
            y = this->pos().y() + ptemp.y();
            w = this->size().width() ;
            h = this->size().height() - ptemp.y();
            if(h < this->minimumHeight())
            {
                m_lastPos.setY(this->pos().y());
            }
            break;
            // 下边框被拉伸
        case Bottom:
            x = this->pos().x();
            y = this->pos().y();
            w = this->size().width() ;
            h = this->size().height() + ptemp.y();
            if(h < this->minimumHeight())
            {
                m_lastPos.setY(this->pos().y() + this->height());
            }
            break;
            //右上角被拉伸
        case TR:
            x = this->pos().x();
            y = this->pos().y() + ptemp.y();
            w = this->size().width() + ptemp.x() ;
            h = this->size().height() - ptemp.y();
            if(w < this->minimumWidth())
            {
                m_lastPos.setX(this->pos().x() + this->width());
            }
            if(h < this->minimumHeight())
            {
                m_lastPos.setY(this->pos().y());
            }
            break;
            //左上角被拉伸
        case TL:
            x = this->pos().x() + ptemp.x();
            y = this->pos().y() + ptemp.y();
            w = this->size().width() - ptemp.x() ;
            h = this->size().height() - ptemp.y();
            if(w < this->minimumWidth())
            {
                m_lastPos.setX(this->pos().x());
            }
            if(h < this->minimumHeight())
            {
                m_lastPos.setY(this->pos().y());
            }
            break;
            // 右下角被拉伸
        case BR:
            x = this->pos().x();
            y = this->pos().y();
            w = this->size().width() + ptemp.x() ;
            h = this->size().height() + ptemp.y();
            if(w < this->minimumWidth())
            {
                m_lastPos.setX(this->pos().x() + this->width());
            }
            if(h < this->minimumHeight())
            {
                m_lastPos.setY(this->pos().y() + this->height());
            }
            break;
            // 左下角被拉伸
        case BL:
            x = this->pos().x() + ptemp.x();
            y = this->pos().y();
            w = this->size().width() - ptemp.x() ;
            h = this->size().height() + ptemp.y();
            if(w < this->minimumWidth())
            {
                m_lastPos.setX(this->pos().x());
            }
            if(h < this->minimumHeight())
            {
                m_lastPos.setY(this->pos().y() + this->height());
            }
            break;
        default:
            return;
        }
        // 改变窗口的大小和位置。
        if(w < this->minimumWidth())
        {
            x = this->pos().x();
            w = this->size().width();
        }
        if(h < this->minimumHeight())
        {
            y = this->pos().y();
            h = this->size().height();
        }
        this->setGeometry(x,y,w,h);
    }
}

void FrameWgt::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_isOp = false;
}
