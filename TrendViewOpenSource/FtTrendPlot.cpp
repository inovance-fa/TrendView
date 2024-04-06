#include "FtTrendPlot.h"
#include "TrendRuler.h"
#include "TrendValueAxisSticker.h"
#include "TrendDateTimeAxisTicker.h"
#include "TrendLegendItem.h"
#include "TrendValueAxis.h"
#include "TrendGraph.h"
using namespace Graph;

FtTrendPlot::FtTrendPlot(QWidget* parent)
    : TrendPlot(parent)
{
}

void FtTrendPlot::SetIsShowXAxis(bool isShow)
{
    xAxis->setVisible(isShow);
}

void FtTrendPlot::SetXAxisDataTimeFormat(int format, int hourFormat)
{
    //组合X轴时间格式字符串
    QString hourFormatStr = hourFormat == 1 ? "AP" : "";
    QString formatStr;
    if (1 == format)
    {
        formatStr = QString("yyyy-MM-dd\nhh:mm:ss") + hourFormatStr;
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
        formatStr = "yyyy-MM-dd\nhh:mm:ss";
    }
    //设置X轴ticker时间格式
    m_pXAxisTicker->setDateTimeFormat(formatStr);
    xAxis->setTicker(m_pXAxisTicker);
    if (nullptr != m_pTrendRuler)
    {
        m_pTrendRuler->setDateTimeFormat(formatStr);
    }
}

void FtTrendPlot::SetXAxisTimeRange(int timeRange, int timeUnit, bool isRunning)
{
    QDateTime dateTime;
    isRunning ? dateTime = QDateTime::currentDateTime()
              : dateTime = QDateTime::fromString("2023-03-21 08:00:00", "yyyy-MM-dd hh:mm:ss");
    double lower = 0.0;
    if (1 == timeUnit)
    {
        lower = dateTime.addSecs(-timeRange).toSecsSinceEpoch();
    }
    else if (2 == timeUnit)
    {
        lower = dateTime.addSecs(-timeRange * 60).toSecsSinceEpoch();
    }
    else if (3 == timeUnit)
    {
        lower = dateTime.addSecs(-timeRange * 3600).toSecsSinceEpoch();
    }
    double upper = dateTime.toSecsSinceEpoch();
    xAxis->setRange(lower, upper);
}

void FtTrendPlot::SetXAxisTimeUnit(int timeRange, int timeUnit)
{
    SetXAxisTimeRange(timeRange, timeUnit);
}

void FtTrendPlot::SetAxisFontColor(const QRgb& wordsColor)
{
    xAxis->setTickLabelColor(GlobalDefine::GetQColorFromQRGB(wordsColor));
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

void FtTrendPlot::SetAxisLabelFont(QFont& font)
{
    xAxis->setLabelFont(font);
    yAxis->setLabelFont(font);
    yAxis2->setLabelFont(font);
}

void FtTrendPlot::SetRulerFont(QFont& font)
{
    m_pTrendRuler->SetRulerFont(font);
}

void FtTrendPlot::SetTrendGraphVisible(const std::map<std::string, bool>& trendVisibleMap)
{
    for (auto graph : mGraphs)
    {
        if (nullptr == graph)
        {
            continue;
        }
        auto name = graph->name();
        auto it = trendVisibleMap.find(name.toStdString());
        if (it != trendVisibleMap.end())
        {
            graph->setVisible(it->second);
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

void FtTrendPlot::SetPlotConfig(const QList<TrendPlotOpenInfo>& plotConfigList)
{
    clearGraphs();
    QMap<QCPGraph*, bool> graphToVarType;
    for (const auto& plotConfig : plotConfigList)
    {
        auto bindYAxis = plotConfig.axisType == 1 ? yAxis : yAxis2;
        auto graph = AddGraph(xAxis, bindYAxis);
        if (nullptr == graph)
        {
            continue;
        }
        graph->setName(QString::fromStdString(plotConfig.trendName));
        graph->setPen(plotConfig.linePen);

        QMap<int, QCPGraph::LineStyle> lineTypeMap{ {1, QCPGraph::lsNone}, 
            {2, QCPGraph::lsLine}, {3, QCPGraph::lsStepLeft} };
        graph->setLineStyle(lineTypeMap.value(plotConfig.trendPlotStyle));
        QCPScatterStyle style(QCPScatterStyle::ScatterShape::ssCircle, plotConfig.linePen.color(), plotConfig.linePen.color(), plotConfig.lineWidth * 2);
        graph->setScatterStyle(style);
        graph->setAdaptiveSampling(true);

        graphToVarType.insert(graph, plotConfig.isBoolType);
    }
    m_pTrendRuler->SetGraphVarTypeMap(graphToVarType);
    InitialLegendBar();
}

QCPGraph* FtTrendPlot::AddGraph(QCPAxis* keyAxis, QCPAxis* valueAxis)
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
    auto newGraph = new TrendGraph(keyAxis, valueAxis);
    newGraph->setName(QLatin1String("Graph ") + QString::number(mGraphs.size()));
    return newGraph;
}

void FtTrendPlot::UpdateTrendsData(const QCPGraph* graph, const std::vector<QPair<std::uint64_t, double>>& values)
{
    auto dataContainer = graph->data();
    for (const auto& data : values)
    {
        QCPGraphData newPoint;
        newPoint.key = double(data.first) / 1000.0;
        newPoint.value = data.second;
        dataContainer->add(newPoint);
    }
    
}

void FtTrendPlot::ClearGraphData(double time)
{
    for (const auto& graph : mGraphs)
    {
        if (nullptr == graph)
        {
            continue;
        }
        auto dataContainer = graph->data();
        if (dataContainer.isNull())
        {
            continue;
        }
        auto beginIt = dataContainer->findBegin(time);
        if (beginIt == dataContainer->constEnd())
        {
            continue;
        }
        dataContainer->removeBefore(beginIt->key);
    }
}

void FtTrendPlot::ClearGraphData()
{
    for (const auto& graph : mGraphs)
    {
        if (nullptr == graph)
        {
            continue;
        }
        auto dataContainer = graph->data();
        if (dataContainer.isNull())
        {
            continue;
        }
        dataContainer->clear();
        dataContainer->squeeze();
    }
    replot();
}

double FtTrendPlot::GetGraphDataFirstOrLastTime(int isForward) const
{
    QList<double> lastTimeList;
    for (const auto& graph : mGraphs)
    {
        if (nullptr == graph)
        {
            continue;
        }
        bool isFound = false;
        auto timeRange = graph->getKeyRange(isFound);
        if (!isFound)
        {
            continue;
        }
        isForward == 0 ? lastTimeList.append(timeRange.lower) : lastTimeList.append(timeRange.upper);
    }
    std::sort(lastTimeList.begin(), lastTimeList.end(), std::greater<double>());
    if (lastTimeList.size() <= 0)
    {
        return -1;
    }
    return isForward == 0 ? lastTimeList.at(lastTimeList.size() - 1) : lastTimeList.at(0);
}

double FtTrendPlot::GetGraphDataTime(const std::string& trendName, int isForward)
{
    for (const auto& graph : mGraphs)
    {
        if (nullptr == graph || graph->name() != QString::fromStdString(trendName))
        {
            continue;
        }

        bool isFound = false;
        auto timeRange = graph->getKeyRange(isFound);
        if (!isFound)
        {
            continue;
        }
        return isForward == 0 ? timeRange.lower : timeRange.upper;
    }
    return 0;
}

void FtTrendPlot::UpdateGraphVarTypeMap(const QList<QPair<std::string, bool>>& infoList)
{
    QMap<QCPGraph*, bool> graphToVarType;
    m_pTrendRuler->GetGraphVarTypeMap(graphToVarType);

    for (auto it = graphToVarType.begin(); it != graphToVarType.end(); ++it)
    {
        if (nullptr == it.key())
        {
            continue;
        }
        for (const auto& configInfo : infoList)
        {
            if (configInfo.first == it.key()->name().toStdString())
            {
                it.value() = configInfo.second;
                break;
            }
        }
    }
    m_pTrendRuler->SetGraphVarTypeMap(graphToVarType);
}

void FtTrendPlot::UpdateXAxisRange(double lower, double upper)
{
    xAxis->setRange(lower, upper);
}

void FtTrendPlot::InitialAxis()
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
    //设置Y轴名称
    yAxis->setLabel(QString::fromUtf8("左Y轴"));
    yAxis2->setLabel(QString::fromUtf8("右Y轴"));
    //设置默认量程上下限
    yAxis->setRange(0, 100);
    yAxis2->setRange(0, 100);
    m_pLYAxisTicker = QSharedPointer<TrendValueAxisSticker>(new TrendValueAxisSticker);
    m_pRYAxisTicker = QSharedPointer<TrendValueAxisSticker>(new TrendValueAxisSticker);
    m_pLYAxisTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssMeetTickCount);
    m_pRYAxisTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssMeetTickCount);
    m_pLYAxisTicker->setScaleStrategy(QCPAxisTickerFixed::ScaleStrategy::ssMultiples);
    m_pRYAxisTicker->setScaleStrategy(QCPAxisTickerFixed::ScaleStrategy::ssMultiples);
    //设置默认主次刻度
    SetLeftYAxisMainTick(20, 4);
    SetRightYAxisMainTick(20, 4);
    SetLeftYAxisSubTick(20, 4);
    SetRightYAxisSubTick(20, 4);
    //轴矩形
    axisRect()->setBackgroundScaled(true);
    axisRect()->setBackgroundScaledMode(Qt::AspectRatioMode::IgnoreAspectRatio);
    axisRect()->setAutoMargins(QCP::msAll);
    axisRect()->setMinimumMargins(MINMARGINS);
    //设置X轴为时间格式
    m_pXAxisTicker = QSharedPointer<TrendDateTimeAxisTicker>(new TrendDateTimeAxisTicker);
    SetXAxisDataTimeFormat(1, 2);
    SetXAxisTimeRange(1, 2);
    yAxis->setPadding(0);
    yAxis2->setPadding(0);
    yAxis->setLabelPadding(2);
    yAxis2->setLabelPadding(2);
    xAxis->setTickLabelPadding(8);
    yAxis->setTickLabelPadding(8);
    yAxis2->setTickLabelPadding(8);
    yAxis2->setLabelColor(Qt::transparent);
}

void FtTrendPlot::InitialLegendBar()
{
    int itemColumn = 0;
    int itemCount = legend->itemCount();
    m_legendItems.clear();
    for (int i = 0; i < itemCount; ++i)
    {
        int itemRow = i / 4;
        if (legend->hasItem(legend->item(i)))
        {
            auto plotLegendItemPtr = dynamic_cast<QCPPlottableLegendItem*>(legend->item(i));
            QCPAbstractPlottable* plottable = nullptr;
            if (plotLegendItemPtr)
            {
                plottable = plotLegendItemPtr->plottable();
            }
            if (nullptr == plottable)
            {
                continue;
            }
            legend->remove(legend->item(i));
            auto item = new TrendLegendItem(legend, plottable);
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

void FtTrendPlot::InitialRuler()
{
    if (nullptr == m_pTrendRuler)
    {
        m_pTrendRuler = new TrendRuler(this);
    }
    m_pTrendRuler->SetIsShow(true);
}

void FtTrendPlot::UpdateRulerPosition()
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
    m_pTrendRuler->UpdatePosition(m_curPos.x(), m_curPos.y());
}

void FtTrendPlot::UpdateRulerPosition(const QPoint& pos)
{
    if (nullptr == m_pTrendRuler)
    {
        return;
    }

    m_curPos = MousePosConvertToRealPos(pos);
    m_pTrendRuler->UpdatePosition(m_curPos.x(), m_curPos.y());
}

void Graph::FtTrendPlot::UpdateRulerYPosition()
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
    m_pTrendRuler->UpdatePosition(m_curPos.x(), m_curPos.y());
}

void FtTrendPlot::SetMainTickCountByControlHeight(int leftYAxisSubTickCnt, int rightYAxisSubTickCnt)
{
    auto axisRectHeight = axisRect()->height();
    auto mainTickCnt = qCeil((double)axisRectHeight / 40);
    if (m_isLAxisAutoValue)
    {
        m_pLYAxisTicker->SetMainTickCount(mainTickCnt + 1);
        m_pLYAxisTicker->SetSubTickCount(leftYAxisSubTickCnt);
    }
    if (m_isRAxisAutoValue)
    {
        m_pRYAxisTicker->SetMainTickCount(mainTickCnt + 1);
        m_pRYAxisTicker->SetSubTickCount(rightYAxisSubTickCnt);
    }
}
