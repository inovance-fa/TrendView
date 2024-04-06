#include "TrendPlot.h"
#include "TrendValueAxisSticker.h"
#include "TrendValueAxis.h"
#include "TrendRuler.h"

using namespace Graph;

TrendPlot::TrendPlot(QWidget* parent)
    : QCustomPlot(parent)
{
}

void TrendPlot::Initial()
{
    //初始化轴
    InitialAxis();
    //初始化轴刻度线
    InitialTick();
    //初始化标尺
    InitialRuler();
    //初始化图例
    InitialLegendBar();
    legend->setIconSize(54, 18);
    setNoAntialiasingOnDrag(true);
    setPlottingHint(QCP::phFastPolylines);
    setPlottingHint(QCP::phCacheLabels);
    replot();
}

void TrendPlot::SetIsShowRuler(bool isShow)
{
    //设置标尺是否显示
    if (nullptr != m_pTrendRuler)
    {
        m_pTrendRuler->SetIsShow(isShow);
    }
    //更新标尺位置
    UpdateRulerYPosition();
}

void TrendPlot::SetIsShowTickLabels(bool isShow)
{
    //设置各轴是否显示刻度标签
    xAxis->setTickLabels(isShow);
    yAxis->setTickLabels(isShow);
    yAxis2->setTickLabels(isShow);
}

void TrendPlot::SetDrawStartPoint(DrawStartPoint startPoint)
{
    bool isReversed = startPoint == DrawStartPoint::Left ? false : true;
    //x轴反向
    xAxis->setRangeReversed(isReversed);
}

void TrendPlot::SetIsShowLeftYAxis(bool isShow)
{
    if (isShow)
    {
        QPen pen;
        pen.setColor(QColor("#A5ADBD"));
        pen.setWidth(1);
        yAxis->setLabelColor(QColor("#0C1A34)"));
        yAxis->setBasePen(QPen(QBrush(QColor("#A5ADBD")), 1));
        yAxis->setTickPen(pen);
        yAxis->setSubTickPen(pen);
        yAxis->setTickLabelColor(GlobalDefine::GetQColorFromQRGB(m_curWordsColor));
    }
    else
    {
        yAxis->setLabelColor(Qt::transparent);
        yAxis->setBasePen(Qt::NoPen);
        yAxis->setTickPen(Qt::NoPen);
        yAxis->setSubTickPen(Qt::NoPen);
        yAxis->setTickLabelColor(QColor(Qt::transparent));
    }

    m_leftIsVisible = isShow;
}

void TrendPlot::SetLeftYAxisAutoRange(bool isAuto)
{
    m_isLAxisAutoValue = isAuto;
    m_pLYAxisTicker->SetIsAutoTick(isAuto);
    if (m_isLAxisAutoValue)
    {
        yAxis->rescale(isAuto);
    }
}

void TrendPlot::SetLeftYAxisName(const QString& name)
{
    SetAxisLabel(yAxis, name);
}

void TrendPlot::SetLeftYAxisStartPoint(double startPoint)
{
    yAxis->setRangeLower(startPoint);
}

void TrendPlot::SetLeftYAxisEndPoint(double endPoint)
{
    yAxis->setRangeUpper(endPoint);
}

void TrendPlot::SetLeftYAxisMainTick(double mainTick, int subTick)
{
    m_pLYAxisTicker->SetMainTickStep(mainTick);
    m_pLYAxisTicker->SetMainTickCount(qCeil((yAxis->range().upper - yAxis->range().lower) / mainTick) + 1);
    m_pLYAxisTicker->SetSubTickCount(subTick);
    yAxis->setTicker(m_pLYAxisTicker);
}

void TrendPlot::SetLeftYAxisMainTick(double mainTick, int subTick, double lower, double upper)
{
    m_pLYAxisTicker->SetMainTickStep(mainTick);
    m_pLYAxisTicker->SetMainTickCount(qCeil((upper - lower) / mainTick) + 1);
    m_pLYAxisTicker->SetSubTickCount(subTick);
    yAxis->setTicker(m_pLYAxisTicker);
}

void TrendPlot::SetLeftYAxisSubTick(double mainTick, int subTick)
{
    m_pLYAxisTicker->SetMainTickCount(qCeil((yAxis->range().upper - yAxis->range().lower) / mainTick) + 1);
    m_pLYAxisTicker->SetMainTickStep(mainTick);
    m_pLYAxisTicker->SetSubTickCount(subTick);
    yAxis->setTicker(m_pLYAxisTicker);
}

void TrendPlot::SetLeftYAxisDecimalPlaces(int decimalPlaces)
{
    m_pLYAxisTicker->SetDecimalPlaces(decimalPlaces);
}

void TrendPlot::SetIsShowRightYAxis(bool isShow)
{
    if (isShow)
    {
        QPen pen;
        pen.setColor(QColor("#A5ADBD"));
        pen.setWidth(1);
        yAxis2->setLabelColor(QColor("#0C1A34)"));
        yAxis2->setBasePen(QPen(QBrush(QColor("#A5ADBD")), 1));
        yAxis2->setTickPen(pen);
        yAxis2->setSubTickPen(pen);
        yAxis2->setTickLabelColor(GlobalDefine::GetQColorFromQRGB(m_curWordsColor));
    }
    else
    {
        yAxis2->setLabelColor(Qt::transparent);
        yAxis2->setBasePen(Qt::NoPen);
        yAxis2->setTickPen(Qt::NoPen);
        yAxis2->setSubTickPen(Qt::NoPen);
        yAxis2->setTickLabelColor(QColor(Qt::transparent));
    }
    m_rightIsVisible = isShow;
}

void TrendPlot::SetRightYAxisAutoRange(bool isAuto)
{
    m_isRAxisAutoValue = isAuto;
    m_pRYAxisTicker->SetIsAutoTick(isAuto);
    if (m_isRAxisAutoValue)
    {
        yAxis2->rescale(isAuto);
    }
}

void TrendPlot::SetRightYAxisName(const QString& name)
{
    SetAxisLabel(yAxis2, name);
}

void TrendPlot::SetRightYAxisStartPoint(double startPoint)
{
    yAxis2->setRangeLower(startPoint);
}

void TrendPlot::SetRightYAxisEndPoint(double endPoint)
{
    yAxis2->setRangeUpper(endPoint);
}

void TrendPlot::SetRightYAxisMainTick(double mainTick, int subTick)
{
    m_pRYAxisTicker->SetMainTickStep(mainTick);
    m_pRYAxisTicker->SetMainTickCount(qCeil((yAxis2->range().upper - yAxis2->range().lower) / mainTick) + 1);
    m_pRYAxisTicker->SetSubTickCount(subTick);
    yAxis2->setTicker(m_pRYAxisTicker);
}

void TrendPlot::SetRightYAxisMainTick(double mainTick, int subTick, double lower, double upper)
{
    m_pRYAxisTicker->SetMainTickStep(mainTick);
    m_pRYAxisTicker->SetMainTickCount(qCeil((upper - lower) / mainTick) + 1);
    m_pRYAxisTicker->SetSubTickCount(subTick);
    yAxis2->setTicker(m_pRYAxisTicker);
}

void TrendPlot::SetRightYAxisSubTick(double mainTick, int subTick)
{
    m_pRYAxisTicker->SetMainTickCount(qCeil((yAxis2->range().upper - yAxis2->range().lower) / mainTick) + 1);
    m_pRYAxisTicker->SetMainTickStep(mainTick);
    m_pRYAxisTicker->SetSubTickCount(subTick);
    yAxis2->setTicker(m_pRYAxisTicker);
}

void TrendPlot::SetRightYAxisDecimalPlaces(int decimalPlaces)
{
    m_pRYAxisTicker->SetDecimalPlaces(decimalPlaces);
}

void TrendPlot::SetBackgroundColor(const QRgb& color)
{
    axisRect()->setBackground(QBrush(QColor(GlobalDefine::GetQColorFromQRGB(color))));
    legend->setBrush(QBrush(QColor(GlobalDefine::GetQColorFromQRGB(color))));
    setBackground(GlobalDefine::GetQColorFromQRGB(color));
}

void TrendPlot::SetRulerColor(const QRgb& color)
{
    m_pTrendRuler->SetRulerColor(color);
}

void TrendPlot::InitCustomValueAxis()
{
    //删除原有左右Y轴
    axisRect()->removeAxis(xAxis);
    axisRect()->removeAxis(yAxis);
    axisRect()->removeAxis(yAxis2);
    //增加自定义下X轴
    AddCustomValueAxis(QCPAxis::atBottom);
    //增加自定义左右Y轴
    AddCustomValueAxis(QCPAxis::atLeft);
    AddCustomValueAxis(QCPAxis::atRight);
    //初始化左右Y轴
    axisRect()->setRangeDragAxes(xAxis, yAxis);
    axisRect()->setRangeZoomAxes(xAxis, yAxis);
    xAxis->grid()->setVisible(true);
    yAxis2->setVisible(false);
    yAxis->grid()->setVisible(true);
    yAxis2->grid()->setVisible(false);
    yAxis2->grid()->setZeroLinePen(Qt::NoPen);
    yAxis2->grid()->setVisible(false);
}

void TrendPlot::AddCustomValueAxis(QCPAxis::AxisType type)
{
    auto newAxis = new TrendValueAxis(axisRect(), type);
    axisRect()->addAxis(type, newAxis);
}

void TrendPlot::InitialTick()
{
    auto f = [](QCPAxis* axis) {
        //刻度线长度和颜色
        QPen pen;
        pen.setColor(QColor("#A5ADBD"));
        pen.setWidth(1);
        axis->setTickPen(pen);
        axis->setSubTickPen(pen);
        axis->setTickLabels(true);
        axis->setTickLengthIn(0);
        axis->setTickLengthOut(5);
        axis->setSubTickLengthIn(0);
        axis->setSubTickLengthOut(3);
    };

    f(xAxis);
    f(yAxis);
    f(yAxis2);
    yAxis2->setTickPen(Qt::NoPen);
    yAxis2->setSubTickPen(Qt::NoPen);
    yAxis2->setTickLabelColor(QColor(Qt::transparent));
}

void TrendPlot::InitialRuler()
{
    if (nullptr == m_pTrendRuler)
    {
        m_pTrendRuler = new TrendRuler(this);
    }
    m_pTrendRuler->SetIsShow(true);
}

void TrendPlot::InitialRulerDataTracer()
{
    if (nullptr != m_pTrendRuler)
    {
        m_pTrendRuler->Initial();
        InitialRulerPos();
    }
}

void TrendPlot::InitialRulerPos()
{
    if (nullptr != m_pTrendRuler)
    {
        UpdateRulerPosition(axisRect()->center());       
        m_rulerCurPos = axisRect()->center();
    }
}

void TrendPlot::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        auto item = itemAt(event->pos(), true);
        if (nullptr != item)
        {
            emit mousePress(event);

            m_bMousePress = true;
            SetRulerSelected(true);
            item->layer()->replot();
            return;
        }
    }
    QCustomPlot::mousePressEvent(event);
}

void TrendPlot::mouseMoveEvent(QMouseEvent* event)
{
    QCustomPlot::mouseMoveEvent(event);
    if (m_pTrendRuler->GetIsRunning())
    {
        auto item = itemAt(event->pos(), true);
        auto cursor = item ? QCursor(Qt::SizeHorCursor) : QCursor(Qt::ArrowCursor);
        if (m_updateCursorFunc)
        {
            m_updateCursorFunc(cursor);
        }
    }

    if (m_bMousePress)
    {     
        auto rect = axisRect()->rect();
        auto xAxisRect = xAxis->axisRect()->rect();
        rect = rect.intersected(xAxisRect);
        auto yAxisRect = yAxis->axisRect()->rect();
        rect = rect.intersected(yAxisRect);
        auto y2AxisRect = yAxis2->axisRect()->rect();
        rect = rect.intersected(y2AxisRect);

        if (event->pos().x() >= rect.x()
            && (event->pos().x() <= rect.x() + rect.width()))
        {
            m_rulerCurPos = event->pos();
            UpdateRulerPosition(m_rulerCurPos);
        }
    }
}

void TrendPlot::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_bMousePress)
    {
        m_bMousePress = false;

        auto item = itemAt(event->pos(), true);
        if (nullptr != item)
        {
            emit mouseRelease(event);
            SetRulerSelected(false);
            item->layer()->replot();
            return;
        }
    }

    QCustomPlot::mouseReleaseEvent(event);
}

QPointF TrendPlot::MousePosConvertToRealPos(const QPointF& pos)
{
    //位置坐标转换为实际轴坐标
    auto xTrendAxisPtr = dynamic_cast<TrendValueAxis*>(xAxis);
    auto yTrendAxisPtr = dynamic_cast<TrendValueAxis*>(yAxis);
    if (xTrendAxisPtr != nullptr && yTrendAxisPtr != nullptr)
    {
        return QPointF(xTrendAxisPtr->pixelToCoord(pos.x()), yTrendAxisPtr->pixelToCoord(pos.y()));
    }
    else
    {
        return QPointF();
    }
}

void TrendPlot::SetRulerSelected(bool selected)
{
    if (nullptr == m_pTrendRuler)
    {
        return;
    }
    m_pTrendRuler->SetSelected(selected);
}

void TrendPlot::SetAxisLabel(QCPAxis* axis, const QString& name)
{
    if (nullptr == axis)
    {
        return;
    }

    auto axisH = axis->axisRect()->height();
    QFontMetrics fontMetrics(axis->labelFont());
    auto labelBounds = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip, name);
    int length = name.length();
    while (labelBounds.width() > axisH)
    {
        length -= 1;
        if (0 >= length)
        {
            break;
        }
        labelBounds = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip, name.left(length) + "...");
    }

    axis->setLabel(GlobalDefine::ElideText(length + 1, name));
}

void TrendPlot::wheelEvent(QWheelEvent* e)
{
     //只在运行态限制画面的滚动事件触发，开发态不限制 BUG：123357
    if (m_pTrendRuler->GetIsRunning())
    {
        QCustomPlot::wheelEvent(e);
    }
}


void TrendPlot::SetIsRunning(bool isRun)
{
    if (nullptr != m_pTrendRuler)
    {
        m_pTrendRuler->SetIsRunning(isRun);
    }
}