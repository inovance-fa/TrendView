#include "qcustomplot.h"
#include "TrendTracer.h"
#include "TrendTracerLine.h"
#include "TrendRuler.h"

#include "TrendRulerItemText.h"
using namespace Graph;
#define USE_DATATRACER false
TrendRuler::TrendRuler(QCustomPlot* plot, QObject* parent)
    : QObject(parent)
    , m_pPlot(plot)
{
    if (!USE_DATATRACER)
    {
        InitialRulerText();
    }
    else
    {
        m_xAsixTracer = new TrendTracer(m_pPlot, TrendTracer::XAxisTracer, this);
    }

    m_pTracerLine = new TrendTracerLine(m_pPlot);
    SetRulerColor(qRgb(0x71, 0x71, 0x71));
}

TrendRuler::~TrendRuler()
{
    for (auto tracer : m_tracerList)
    {
        if (nullptr == tracer)
        {
            continue;
        }
        delete tracer;
        tracer = nullptr;
    }

    if (nullptr != m_xAsixTracer)
    {
        delete m_xAsixTracer;
        m_xAsixTracer = nullptr;
    }

    if (nullptr != m_pTracerLine)
    {
        m_pPlot->removeItem(m_pTracerLine);
    }
}

void TrendRuler::Initial()
{
    if (USE_DATATRACER)
    {
        if (nullptr == m_pPlot)
        {
            return;
        }
        auto graphCount = m_pPlot->graphCount();
        for (int i = 0; i < graphCount; ++i)
        {
            auto tracer = new TrendTracer(m_pPlot, TrendTracer::DataTracer, this);
            tracer->SetColor(m_pPlot->graph(i)->pen().color().rgba());
            tracer->SetBrush(Qt::NoBrush);
            tracer->SetLabelPen(m_pPlot->graph(i)->pen());
            tracer->SetVisible(true);
            m_bGraphVisibleList.append(true);
            m_tracerList.append(tracer);
        }
    }
}

void TrendRuler::InitialRulerText()
{
    m_pRulerItemText = new TrendRulerItemText(m_pPlot);
    m_pRulerItemText->setLayer("overlay");
    m_pRulerItemText->setClipToAxisRect(false);
    m_pRulerItemText->setPadding(QMargins(5, 5, 5, 5));
    m_pRulerItemText->setBrush(Qt::NoBrush);
    m_pRulerItemText->setPen(QPen(QColor("#0C1A34")));
    m_pRulerItemText->setFont(QFont("宋体", 12));
    m_pRulerItemText->setColor(QColor("#0C1A34"));
    m_pRulerItemText->setText("");
    m_pRulerItemText->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);
    m_pRulerItemText->setSelectable(false);
    m_pRulerItemText->position->setTypeX(QCPItemPosition::ptPlotCoords);
    m_pRulerItemText->position->setTypeY(QCPItemPosition::ptPlotCoords);
    m_pRulerItemText->position->setCoords(m_pPlot->xAxis->range().lower, m_pPlot->yAxis->range().upper);
}

void TrendRuler::SetRulerColor(const QRgb& rgb)
{
    if (m_pTracerLine)
    {
        m_pTracerLine->SetColor(rgb);
    }
    if (m_pRulerItemText)
    {
        m_pRulerItemText->SetRulerColor(rgb);
    }
    if (m_xAsixTracer)
    {
        m_xAsixTracer->SetColor(rgb);
    }
}

void TrendRuler::SetRulerFont(const QFont& font)
{
    if (USE_DATATRACER)
    {
        for (auto tracer : m_tracerList)
        {
            if (tracer)
            {
                tracer->SetTracerFont(font);
            }
        }
        if (m_xAsixTracer)
        {
            m_xAsixTracer->SetTracerFont(font);
        }
    }
    else
    {
        if (m_pRulerItemText)
        {
            m_pRulerItemText->SetRulerFont(font);
        }
    }
}

void TrendRuler::SetIsShow(bool isShow)
{
    m_isShow = isShow;
    if (m_pTracerLine)
    {
        m_pTracerLine->SetVisible(isShow);
    }
    if (USE_DATATRACER)
    {
        for (int i = 0; i < m_tracerList.size(); ++i)
        {
            if (m_bGraphVisibleList[i])
            {
                m_tracerList[i]->SetVisible(isShow);
            }
        }

        if (m_xAsixTracer)
        {
            m_xAsixTracer->SetVisible(isShow);
        }
    }
    else
    {
        if (m_pRulerItemText)
        {
            m_pRulerItemText->SetVisible(isShow);
        }
    }
    if (m_pTracerLine)
    {
        m_pTracerLine->layer()->replot();
    }
}

void TrendRuler::UpdatePosition(qreal xValue, qreal yValue)
{
    if (m_pTracerLine)
    {
        m_pTracerLine->UpdatePosition(xValue);
    }
    if (!USE_DATATRACER)
    {
        RulerData ruleData;
        QList<RulerData::TrendDisplayData> displayDataList;
        ruleData.time = QDateTime::fromSecsSinceEpoch(xValue).toString(m_formatStr);
        auto graphCount = m_pPlot->graphCount();
        for (int i = 0; i < graphCount; ++i)
        {
            RulerData::TrendDisplayData plotData;
            plotData.lineColor = m_pPlot->graph(i)->pen().color().rgb();
            plotData.trendName = m_pPlot->graph(i)->name();
            plotData.isVisible = m_pPlot->graph(i)->visible();
            bool foundRange = false;
            auto keyRange = m_pPlot->graph(i)->data()->keyRange(foundRange);
            if (!foundRange || xValue < keyRange.lower || xValue > keyRange.upper)
            {
                plotData.data = "???";
            }
            else
            {
                auto nextIt = m_pPlot->graph(i)->data()->findEnd(xValue, false);
                auto beginIt = m_pPlot->graph(i)->data()->findBegin(xValue);
                auto nextKey = nextIt->key;
                auto nextValue = nextIt->value;
                auto beginKey = beginIt->key;
                auto beginValue = beginIt->value;
                if (qFuzzyCompare(nextKey, beginKey))
                {
                    plotData.data = QString::number(nextValue);
                }
                else
                {
                    if (m_pPlot->graph(i)->lineStyle() == QCPGraph::lsLine
                        || m_pPlot->graph(i)->lineStyle() == QCPGraph::lsNone)
                    {
                        if (!m_graphToVarType.contains(m_pPlot->graph(i)))
                        {
                            continue;
                        }
                        if (!m_graphToVarType.value(m_pPlot->graph(i)))
                        {
                            auto ratio = (nextValue - beginValue) / (nextKey - beginKey);
                            auto dataValue = (xValue - beginKey) * ratio + beginValue;
                            plotData.data = QString::number(dataValue);
                        }
                        else
                        {
                            plotData.data = QString::number(beginValue);
                        }
                    }
                    else if (m_pPlot->graph(i)->lineStyle() == QCPGraph::lsStepLeft)
                    {
                        plotData.data = QString::number(beginValue);
                    }
                }
            }
            displayDataList.append(plotData);
        }
        ruleData.displayDataList = displayDataList;

        m_pRulerItemText->SetRulerData(ruleData);
        m_pRulerItemText->position->setCoords(xValue, m_pPlot->yAxis->range().upper);
        m_pRulerItemText->layer()->replot();
    }
    else
    {
        if (m_xAsixTracer)
        {
            m_xAsixTracer->UpdatePosition(xValue, yValue, false);
        }
        auto graphCount = m_pPlot->graphCount();
        for (int i = 0; i < graphCount; ++i)
        {
            if (m_tracerList.size() - 1 < i)
            {
                continue;
            }
            auto tracer = m_tracerList.at(i);
            if (nullptr == tracer)
            {
                continue;
            }
            QVector<double> vector;
            bool isFound = false;
            auto it = m_pPlot->graph(i)->data()->constBegin();
            while (it != m_pPlot->graph(i)->data()->constEnd())
            {
                vector.push_back(it->mainKey());
                if (qint64(it->mainKey()) == qint64(xValue))
                {
                    isFound = true;
                    break;
                }
                ++it;
            }
            tracer->UpdatePosition(xValue, m_pPlot->graph(i)->data()->findBegin(xValue)->value, !isFound);
            bool isVisible = m_bGraphVisibleList.at(i);
            auto valueAxis = m_pPlot->graph(i)->valueAxis() == m_pPlot->yAxis ? m_pPlot->yAxis : m_pPlot->yAxis2;
            if (nullptr == valueAxis)
            {
                continue;
            }
            if (m_pPlot->graph(i)->data()->findBegin(xValue)->value < valueAxis->range().lower)
            {
                isVisible = false;
            }
            tracer->SetVisible(isVisible);
        }
    }
}

void TrendRuler::SetSelected(bool isSelect)
{
    if (m_pTracerLine)
    {
        m_pTracerLine->setSelected(isSelect);
    }
    if (USE_DATATRACER)
    {
        if (m_xAsixTracer)
        {
            m_xAsixTracer->SetSelected(isSelect);
        }
        for (auto tracer : m_tracerList)
        {
            if (tracer)
            {
                tracer->SetSelected(isSelect);
            }
        }
    }
    else
    {
        if (m_pRulerItemText)
        {
            m_pRulerItemText->SetSelected(isSelect);
        }
    }
}

void TrendRuler::SetDataTracerVisible(int index, bool visible)
{
    if (nullptr == m_pPlot
        || m_tracerList.size() == 0
        || index < 0
        || m_tracerList.size() - 1 < index)
    {
        return;
    }
    m_tracerList.at(index)->SetVisible(visible);
    m_bGraphVisibleList[index] = visible;
}

void TrendRuler::SetIsRunning(bool isRun)
{
    if (nullptr != m_pRulerItemText)
    {
        m_pRulerItemText->SetIsRunning(isRun);
    }
}

bool TrendRuler::GetIsRunning()
{
    if (nullptr == m_pRulerItemText)
    {
        return false;
    }
    return m_pRulerItemText->GetIsRunning();
}