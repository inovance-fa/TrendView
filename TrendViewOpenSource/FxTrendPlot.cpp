#include "FxTrendPlot.h"
#include "FxTrendRuler.h"
#include "TrendValueAxisSticker.h"
#include "FxTrendLegendItem.h"
#include "TrendValueAxis.h"
#include "TrendCurve.h"
using namespace Graph;

constexpr const int RUNDATA_MAX_SIZE = 5000;
constexpr const int RUNDATA_DEL_SIZE = 4500;

FxTrendPlot::FxTrendPlot(QWidget* parent)
    : TrendPlot(parent)
{
}

void FxTrendPlot::SetIsShowXAxis(bool isShow)
{
    if (isShow)
    {
        QPen pen;
        pen.setColor(QColor("#A5ADBD"));
        pen.setWidth(1);
        xAxis->setLabelColor(QColor("#0C1A34)"));
        xAxis->setBasePen(QPen(QBrush(QColor("#A5ADBD")), 1));
        xAxis->setTickPen(pen);
        xAxis->setSubTickPen(pen);
        xAxis->setTickLabelColor(GlobalDefine::GetQColorFromQRGB(m_curWordsColor));
    }
    else
    {
        xAxis->setLabelColor(Qt::transparent);
        xAxis->setBasePen(Qt::NoPen);
        xAxis->setTickPen(Qt::NoPen);
        xAxis->setSubTickPen(Qt::NoPen);
        xAxis->setTickLabelColor(QColor(Qt::transparent));
    }
   
    m_xIsVisible = isShow;
}

void Graph::FxTrendPlot::SetXAxisAutoRange(bool isAuto)
{
    m_isXAxisAutoValue = isAuto;
    m_pXAxisTicker->SetIsAutoTick(isAuto);
    if (m_isXAxisAutoValue)
    {
        xAxis->rescale(isAuto);
    }
}

void Graph::FxTrendPlot::SetXAxisName(const QString& name)
{
    SetXAxisLabel(xAxis, name);
}

void Graph::FxTrendPlot::SetXAxisStartPoint(double startPoint)
{
    xAxis->setRangeLower(startPoint);
}

void Graph::FxTrendPlot::SetXAxisEndPoint(double endPoint)
{
    xAxis->setRangeUpper(endPoint);
}

void Graph::FxTrendPlot::SetXAxisMainTick(double mainTick, int subTick)
{
    m_pXAxisTicker->SetMainTickStep(mainTick);
    m_pXAxisTicker->SetMainTickCount(qCeil((xAxis->range().upper - xAxis->range().lower) / mainTick) + 1);
    m_pXAxisTicker->SetSubTickCount(subTick);
    xAxis->setTicker(m_pXAxisTicker);
}

void Graph::FxTrendPlot::SetXAxisMainTick(double mainTick, int subTick, double lower, double upper)
{
    m_pXAxisTicker->SetMainTickStep(mainTick);
    m_pXAxisTicker->SetMainTickCount(qCeil((upper - lower) / mainTick) + 1);
    m_pXAxisTicker->SetSubTickCount(subTick);
    xAxis->setTicker(m_pXAxisTicker);
}

void Graph::FxTrendPlot::SetXAxisSubTick(double mainTick, int subTick)
{
    m_pXAxisTicker->SetMainTickCount(qCeil((xAxis->range().upper - xAxis->range().lower) / mainTick) + 1);
    m_pXAxisTicker->SetMainTickStep(mainTick);
    m_pXAxisTicker->SetSubTickCount(subTick);
    xAxis->setTicker(m_pXAxisTicker);
}

void Graph::FxTrendPlot::SetXAxisDecimalPlaces(int decimalPlaces)
{
    m_pXAxisTicker->SetDecimalPlaces(decimalPlaces);
    if (m_pTrendRuler)
    {
        auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
        if (fxTrendRulerPtr)
        {
            fxTrendRulerPtr->SetXAxisPrecision(decimalPlaces);
        }
    }
}

void FxTrendPlot::SetAxisFontColor(const QRgb& wordsColor)
{
    if (m_xIsVisible)
    {
        xAxis->setTickLabelColor(GlobalDefine::GetQColorFromQRGB(wordsColor));
    }
    if (m_leftIsVisible)
    {
        yAxis->setTickLabelColor(GlobalDefine::GetQColorFromQRGB(wordsColor));
    }
    if (m_rightIsVisible)
    {
        yAxis2->setTickLabelColor(GlobalDefine::GetQColorFromQRGB(wordsColor));
    }
    m_curWordsColor = wordsColor;
}

void FxTrendPlot::SetAxisLabelFont(QFont& font)
{
    xAxis->setLabelFont(font);
    yAxis->setLabelFont(font);
    yAxis2->setLabelFont(font);
}

void FxTrendPlot::SetRulerFont(QFont& font)
{
    m_pTrendRuler->SetRulerFont(font);
}

void FxTrendPlot::SetTrendGraphVisible(const std::map<std::string, bool>& trendVisibleMap)
{
    for (auto curve : m_curves)
    {
        if (nullptr == curve)
        {
            continue;
        }
        auto name = curve->name();
        auto it = trendVisibleMap.find(name.toStdString());
        if (it != trendVisibleMap.end())
        {
            curve->setVisible(it->second);
            if (m_legendItems.contains(name))
            {
                auto item = m_legendItems.value(name);
                if (nullptr != item)
                {
                    item->setVisible(it->second);
                }
            }
        }
    }
    //更新标尺位置
    UpdateRulerPosition();
    //更新曲线显示
    replot();
}

void FxTrendPlot::SetPlotConfig(const QList<TrendPlotOpenInfo>& plotConfigList)
{
    ClearCurves();
    for (const auto& plotConfig : plotConfigList)
    {
        auto bindYAxis = plotConfig.axisType == 1 ? yAxis : yAxis2;
        auto curve = AddCurve(xAxis, bindYAxis);
        if (nullptr == curve)
        {
            continue;
        }
        curve->setName(QString::fromStdString(plotConfig.trendName));
        curve->setPen(plotConfig.linePen);

        QMap<int, QCPCurve::LineStyle> lineTypeMap{ {1, QCPCurve::lsNone},
            {2, QCPCurve::lsLine}};
        curve->setLineStyle(lineTypeMap.value(plotConfig.trendPlotStyle));
        QCPScatterStyle style(QCPScatterStyle::ScatterShape::ssCircle, plotConfig.linePen.color(), plotConfig.linePen.color(), plotConfig.lineWidth * 2);
        curve->setScatterStyle(style);
    }
    if (m_pTrendRuler)
    {
        auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
        if (fxTrendRulerPtr)
        {
            fxTrendRulerPtr->SetCurvesParam(m_curves);
        }
    }
    
    InitialLegendBar();
}

void FxTrendPlot::UpdateTrendsData(const QCPCurve* curve, const std::vector<QPair<std::uint64_t, double>>& values)
{
    auto dataContainer = curve->data();
    //配置小数位参数
    auto yTrendTicker = dynamic_cast<TrendValueAxisSticker*>(curve->valueAxis()->ticker().data());
    int yDecimal = (yTrendTicker != nullptr) ? yTrendTicker->GetDecimalPlaces() : 0;
    for (const auto& data : values)
    {
        QCPCurveData newPoint;
        newPoint.key = FilterDataPrecision(data.first, yDecimal);
        newPoint.value = FilterDataPrecision(data.second, yDecimal);
        dataContainer->add(newPoint);
    }
}

void FxTrendPlot::ClearGraphData()
{
    for (const auto& curve : m_curves)
    {
        if (nullptr == curve)
        {
            continue;
        }
        auto dataContainer = curve->data(); 
        if (dataContainer.isNull())
        {
            continue;
        }
        dataContainer->clear();
        dataContainer->squeeze();
    }
    replot();
}

void Graph::FxTrendPlot::ClearGraphEarlyData()
{
    for (const auto& curve : m_curves)
    {
        if (nullptr == curve)
        {
            continue;
        }
        auto dataContainer = curve->data();
        //保留4500个数据
        if (dataContainer->size() > RUNDATA_MAX_SIZE)
        {
            auto index = dataContainer->size() - RUNDATA_DEL_SIZE ;
            auto delIt = dataContainer->at(index);
            dataContainer->removeBefore(delIt->sortKey());
        }
    }
}

double FxTrendPlot::GetGraphDataFirstOrLastTime(int isForward) const
{
    QList<double> lastTimeList;
    for (const auto& curve : m_curves)
    {
        if (nullptr == curve)
        {
            continue;
        }
        bool isFound = false;
        auto keyRange = curve->getKeyRange(isFound);
        if (!isFound)
        {
            continue;
        }
        auto lower = curve->data()->constBegin()->sortKey();
        auto upper = (curve->data()->constEnd() - 1)->sortKey();

        isForward == 0 ? lastTimeList.append(lower) : lastTimeList.append(upper);
    }
    std::sort(lastTimeList.begin(), lastTimeList.end(), std::greater<double>());
    if (lastTimeList.size() <= 0)
    {
        return -1;
    }
    return isForward == 0 ? lastTimeList.at(lastTimeList.size() - 1) : lastTimeList.at(0);
}

double FxTrendPlot::GetGraphDataTime(const std::string& trendName, int isForward)
{
    for (const auto& curve : m_curves)
    {
        if (nullptr == curve || curve->name() != QString::fromStdString(trendName))
        {
            continue;
        }
        if (curve->data().data()->isEmpty())
        {
            return 0;
        }
        bool isFound = false;
        auto keyRange = curve->getKeyRange(isFound);
        if (!isFound)
        {
            continue;
        }
        auto lower = curve->data()->begin()->sortKey();
        auto upper = (curve->data()->end() - 1)->sortKey();

        return isForward == 0 ? lower : upper;
    }
    return 0;
}

void Graph::FxTrendPlot::SetRulerTextHeight(int hei)
{
    if (m_pTrendRuler)
    {
        auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
        if (fxTrendRulerPtr)
        {
            fxTrendRulerPtr->SetRulerTextHeight(hei);
        }
    }
}

void Graph::FxTrendPlot::SetRecipeShow(bool isShow)
{
    if (m_pTrendRuler)
    {
        auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
        if (fxTrendRulerPtr)
        {
            fxTrendRulerPtr->SetRecipeShowRuler(isShow);
        }
    }
}

bool Graph::FxTrendPlot::GetRecipeShow()
{
    if (m_pTrendRuler)
    {
        auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
        if (fxTrendRulerPtr)
        {
            return fxTrendRulerPtr->GetRecipeShow();
        }
    }
    return false;
}

void FxTrendPlot::InitialAxis()
{
    //初始化自定义左右Y轴
    InitCustomValueAxis();
    //设置不显示y轴2
    yAxis2->setVisible(true);
    //设置轴默认颜色
    xAxis->setBasePen(QPen(QBrush(QColor("#A5ADBD")), 1));
    yAxis->setBasePen(QPen(QBrush(QColor("#A5ADBD")), 1));
    yAxis2->setBasePen(Qt::NoPen);
    //设置网格线不显示
    xAxis->grid()->setVisible(false);
    yAxis->grid()->setVisible(false);
    yAxis2->grid()->setVisible(false);
    //设置轴名称
    xAxis->setLabel(QString::fromUtf8("X轴"));
    yAxis->setLabel(QString::fromUtf8("左Y轴"));
    yAxis2->setLabel(QString::fromUtf8("右Y轴"));
    //设置默认量程上下限
    xAxis->setRange(0, 100);
    yAxis->setRange(0, 100);
    yAxis2->setRange(0, 100);
    m_pXAxisTicker = QSharedPointer<TrendValueAxisSticker>(new TrendValueAxisSticker);
    m_pLYAxisTicker = QSharedPointer<TrendValueAxisSticker>(new TrendValueAxisSticker);
    m_pRYAxisTicker = QSharedPointer<TrendValueAxisSticker>(new TrendValueAxisSticker);
    m_pXAxisTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssMeetTickCount);
    m_pLYAxisTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssMeetTickCount);
    m_pRYAxisTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssMeetTickCount);
    m_pXAxisTicker->setScaleStrategy(QCPAxisTickerFixed::ScaleStrategy::ssMultiples);
    m_pLYAxisTicker->setScaleStrategy(QCPAxisTickerFixed::ScaleStrategy::ssMultiples);
    m_pRYAxisTicker->setScaleStrategy(QCPAxisTickerFixed::ScaleStrategy::ssMultiples);
    //设置默认主次刻度
    SetXAxisMainTick(20, 4);
    SetXAxisSubTick(20, 4);
    SetLeftYAxisMainTick(20, 4);
    SetRightYAxisMainTick(20, 4);
    SetLeftYAxisSubTick(20, 4);
    SetRightYAxisSubTick(20, 4);
    //轴矩形
    axisRect()->setBackgroundScaled(true);
    axisRect()->setBackgroundScaledMode(Qt::AspectRatioMode::IgnoreAspectRatio);
    axisRect()->setAutoMargins(QCP::msAll);
    axisRect()->setMinimumMargins(MINMARGINS);
    //设置轴格式
    yAxis->setPadding(0);
    yAxis2->setPadding(0);
    yAxis->setLabelPadding(2);
    yAxis2->setLabelPadding(2);
    xAxis->setTickLabelPadding(8);
    yAxis->setTickLabelPadding(8);
    yAxis2->setTickLabelPadding(8);
    yAxis2->setLabelColor(Qt::transparent);
}

int Graph::FxTrendPlot::ClearCurves()
{
    m_curves.clear();
    return clearPlottables();
}

QCPCurve* Graph::FxTrendPlot::AddCurve(QCPAxis* keyAxis, QCPAxis* valueAxis)
{
    if (nullptr == keyAxis)
    {
        keyAxis = xAxis;
    }
    if (nullptr == valueAxis)
    {
        valueAxis = yAxis;
    }
    if (nullptr == keyAxis
        || nullptr == valueAxis
        || keyAxis->parentPlot() != this
        || valueAxis->parentPlot() != this)
    {
        return nullptr;
    }
    auto newCurve = new TrendCurve(keyAxis, valueAxis);
    if (newCurve)
    {
        m_curves.append(newCurve);
        registerPlottable(newCurve);
    }

    newCurve->setName(QLatin1String("Curve ") + QString::number(m_curves.size()));
    return newCurve;
}

void FxTrendPlot::InitialLegendBar()
{
    int itemColumn = 0;
    int itemCount = legend->itemCount();
    m_legendItems.clear();
    for (int i = 0; i < itemCount; ++i)
    {
        int itemRow = i / 4;
        if (legend->hasItem(legend->item(i)))
        {
            auto legendItem = dynamic_cast<QCPPlottableLegendItem*>(legend->item(i));
            if (nullptr == legendItem)
            {
                continue;
            }

            auto plottable = legendItem->plottable();
            if (nullptr == plottable)
            {
                continue;
            }
            legend->remove(legend->item(i));
            auto item = new FxTrendLegendItem(legend, plottable);
            legend->addElement(itemRow, itemColumn, item);
            m_legendItems.insert(plottable->name(), item);
            itemColumn++;
            if (itemColumn >= 4)
            {
                itemColumn = 0;
            }
        }
        else
        {
            break;
        }
    } 
    legend->simplify();
    int leftYPos = xAxis->axisRect()->left();
    legend->setRowSpacing(8);
    legend->setMargins(QMargins(leftYPos + 12, 1, 1 , 1));
    plotLayout()->take(legend); 
    plotLayout()->simplify();
    if (itemCount > 0)
    {
        legend->setBorderPen(Qt::NoPen);       
        plotLayout()->addElement(1, 0, legend);
        plotLayout()->setRowStretchFactor(1, 0.001);
        legend->setVisible(true);
    }
    else
    {
        legend->setOuterRect(QRect(0, 0, 0, 0));
    }
}

void FxTrendPlot::InitialRuler()
{
    if (nullptr == m_pTrendRuler)
    {
        m_pTrendRuler = new FxTrendRuler(this);
    }
    m_pTrendRuler->SetIsShow(true);
}

double Graph::FxTrendPlot::FilterDataPrecision(double data, int precision)
{
    double factor = (precision == 0) ? 1 : std::pow(10.0, precision);
    return std::round(data * factor) / factor;
}

void FxTrendPlot::UpdateRulerPosition()
{
    if (nullptr == m_pTrendRuler
        || !m_pTrendRuler->IsShow())
    {
        return;
    }
    m_curPos = MousePosConvertToRealPos(m_rulerCurPos);

    if (m_curPos.x() > xAxis->range().upper)
    {
        m_curPos.setX(xAxis->range().upper);
    }
    else if (m_curPos.x() < xAxis->range().lower)
    {
        m_curPos.setX(xAxis->range().lower);
    }
    auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
    if (fxTrendRulerPtr)
    {
        auto xValue = FilterDataPrecision(m_curPos.x(), fxTrendRulerPtr->GetXAxisPrecision());
        fxTrendRulerPtr->UpdatePosition(xValue, m_curPos.y());
    }
}

void FxTrendPlot::UpdateRulerPosition(const QPoint& pos)
{
    if (nullptr == m_pTrendRuler
        || !m_pTrendRuler->IsShow())
    {
        return;
    }

    m_curPos = MousePosConvertToRealPos(pos);
    auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
    if (fxTrendRulerPtr)
    {
        auto xValue = FilterDataPrecision(m_curPos.x(), fxTrendRulerPtr->GetXAxisPrecision());
        fxTrendRulerPtr->UpdatePosition(xValue, m_curPos.y());
    }
}

void Graph::FxTrendPlot::UpdateRulerYPosition()
{
    if (nullptr == m_pTrendRuler
        || !m_pTrendRuler->IsShow())
    {
        return;
    }

    if (m_curPos.x() > xAxis->range().upper)
    {
        m_curPos.setX(xAxis->range().upper);
    }
    else if (m_curPos.x() < xAxis->range().lower)
    {
        m_curPos.setX(xAxis->range().lower);
    }
    auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
    if (fxTrendRulerPtr)
    {
        auto xValue = FilterDataPrecision(m_curPos.x(), fxTrendRulerPtr->GetXAxisPrecision());
        fxTrendRulerPtr->UpdatePosition(xValue, m_curPos.y());
    }
}

void Graph::FxTrendPlot::SetXAxisLabel(QCPAxis* axis, const QString& name)
{
    if (nullptr == axis)
    {
        return;
    }

    auto axisW = axis->axisRect()->width();
    QFontMetrics fontMetrics(axis->labelFont());
    auto labelBounds = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip, name);
    int length = name.length();
    while (labelBounds.width() > axisW)
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

void FxTrendPlot::SetMainTickCountByControlSize(int xAxisSubTickCnt, int leftYAxisSubTickCnt, int rightYAxisSubTickCnt)
{
    auto axisRectHeight = axisRect()->height();
    auto yAxisMainTickCnt = qCeil((double)axisRectHeight / 40);
    if (m_isLAxisAutoValue)
    {
        m_pLYAxisTicker->SetMainTickCount(yAxisMainTickCnt + 1);
        m_pLYAxisTicker->SetSubTickCount(leftYAxisSubTickCnt);
    }
    if (m_isRAxisAutoValue)
    {
        m_pRYAxisTicker->SetMainTickCount(yAxisMainTickCnt + 1);
        m_pRYAxisTicker->SetSubTickCount(rightYAxisSubTickCnt);
    }
    auto axisRectWidth = axisRect()->width();
    auto xAxisMainTickCnt = qCeil((double)axisRectWidth / 40);
    if (m_isXAxisAutoValue)
    {
        m_pXAxisTicker->SetMainTickCount(xAxisMainTickCnt + 1);
        m_pXAxisTicker->SetSubTickCount(xAxisSubTickCnt);
    }
}

void Graph::FxTrendPlot::SetDataTimeFormat(int format, int hourFormat)
{
    //组合时间格式字符串
    QString hourFormatStr = hourFormat == 1 ? "AP" : "";
    QString formatStr;
    if (1 == format)
    {
        formatStr = QString("yyyy-MM-dd hh:mm:ss") + hourFormatStr;
    }
    else if (2 == format)
    {
        formatStr = "hh:mm:ss" + hourFormatStr;
    }
    else if (3 == format)
    {
        formatStr = "mm:ss";
    }
    else if (4 == format)
    {
        formatStr = "ss";
    }
    else
    {
        formatStr = "yyyy-MM-dd hh:mm:ss";
    }
    //设置标尺时间格式
    if (nullptr != m_pTrendRuler)
    {
        auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
        if (fxTrendRulerPtr)
        {
            fxTrendRulerPtr->setDateTimeFormat(formatStr);
        }
    }
}

void Graph::FxTrendPlot::RulerMoveLeft()
{
    if (nullptr == m_pTrendRuler
        || !m_pTrendRuler->IsShow())
    {
        return;
    }
    auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
    if (fxTrendRulerPtr)
    {
        auto xPrecision = fxTrendRulerPtr->GetXAxisPrecision();
        m_curPos.setX(m_curPos.x() - std::pow(10.0, -xPrecision));
    }
    UpdateRulerYPosition();
}

void Graph::FxTrendPlot::RulerMoveRight()
{
    if (nullptr == m_pTrendRuler
        || !m_pTrendRuler->IsShow())
    {
        return;
    }
    auto fxTrendRulerPtr = dynamic_cast<FxTrendRuler*>(m_pTrendRuler);
    if (fxTrendRulerPtr)
    {
        auto xPrecision = fxTrendRulerPtr->GetXAxisPrecision();
        m_curPos.setX(m_curPos.x() + std::pow(10.0, -xPrecision));
    }
    UpdateRulerYPosition();
}
