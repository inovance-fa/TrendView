#include "qcustomplot.h"
#include "TrendTracer.h"
#include "TrendTracerLine.h"
#include "FxTrendRuler.h"
#include "TrendValueAxisSticker.h"
#include "FxTrendRulerItemText.h"
using namespace Graph;
#define USE_DATATRACER false
FxTrendRuler::FxTrendRuler(QCustomPlot* plot, QObject* parent)
    : TrendRuler(plot,parent)
{
    if (m_pRulerItemText)
    {
        plot->removeItem(m_pRulerItemText);
        InitialRulerText();
    }
}

FxTrendRuler::~FxTrendRuler()
{
    
}

void FxTrendRuler::InitialRulerText()
{
    m_pRulerItemText = new FxTrendRulerItemText(m_pPlot);
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

void Graph::FxTrendRuler::SetCurvesParam(const QList<QCPCurve*>& curves)
{
    if (m_pRulerItemText)
    {
        auto fxRulerItemTextPtr = dynamic_cast<FxTrendRulerItemText*>(m_pRulerItemText);
        if (fxRulerItemTextPtr != nullptr)
        {
            fxRulerItemTextPtr->SetCurvesParam(curves);
        }
    }
}

QList<QCPCurve*> Graph::FxTrendRuler::GetCurvesParam()
{
    auto txRulerItemTextPtr = dynamic_cast<FxTrendRulerItemText*>(m_pRulerItemText);
    if (txRulerItemTextPtr != nullptr)
    {
        return txRulerItemTextPtr->GetCurvesParam();
    }
    else
    {
        return QList<QCPCurve*>{};
    }
}

void Graph::FxTrendRuler::SetRulerTextHeight(int hei)
{
    if (m_pRulerItemText)
    {
        auto fxTrendRulerItemTextPtr = dynamic_cast<FxTrendRulerItemText*>(m_pRulerItemText);
        if (fxTrendRulerItemTextPtr != nullptr)
        {
            fxTrendRulerItemTextPtr->SetRulerTextHeight(hei);
        }
    }
}

void Graph::FxTrendRuler::SetRecipeShowRuler(bool isShow)
{
    m_isRecipe = isShow;
}

void FxTrendRuler::UpdatePosition(qreal xValue, qreal yValue)
{
    if (m_pTracerLine)
    {
        m_pTracerLine->UpdatePosition(xValue);
    }
    if (!USE_DATATRACER)
    {
        FxRulerData ruleData;
        QList<FxRulerData::FxTrendDisplayData> displayDataList;
        
        ruleData.xValue = QString::number(xValue,'f', m_xAxisPrecision);
        auto xValueTemp = ruleData.xValue.toDouble();
        auto curveCount = GetCurvesParam().count();
        for (int i = 0; i < curveCount; ++i)
        {
            
            bool foundRange = false;
            auto keyRange = GetCurvesParam().at(i)->data()->keyRange(foundRange);
            auto rangeL = QString::number(keyRange.lower, 'f', m_xAxisPrecision).toDouble();
            auto rangeU = QString::number(keyRange.upper, 'f', m_xAxisPrecision).toDouble();
            if (!foundRange || xValueTemp < rangeL || xValueTemp > rangeU)
            {
                FxRulerData::FxTrendDisplayData plotData;
                plotData.lineColor = GetCurvesParam().at(i)->pen().color().rgb();
                plotData.trendName = GetCurvesParam().at(i)->name();
                plotData.isVisible = GetCurvesParam().at(i)->visible();
                plotData.yValue = "???";
                plotData.source = "???";
                displayDataList.append(plotData);
            }
            else
            {
                auto curveDatas = GetCurvesParam().at(i)->data();
                //配置小数位参数
                auto yTrendTicker = dynamic_cast<TrendValueAxisSticker*>(GetCurvesParam().at(i)->valueAxis()->ticker().data());
                int yDecimal = (yTrendTicker != nullptr) ? yTrendTicker->GetDecimalPlaces() : 0;

                //备用方法
                for (auto itor = curveDatas->constBegin(); itor != curveDatas->constEnd(); itor++)
                {
                    FxRulerData::FxTrendDisplayData plotData;
                    plotData.lineColor = GetCurvesParam().at(i)->pen().color().rgb();
                    plotData.trendName = GetCurvesParam().at(i)->name();
                    plotData.isVisible = GetCurvesParam().at(i)->visible();

                    if (qFuzzyCompare(itor->mainKey(), xValueTemp))
                    {
                        plotData.yValue = QString::number(itor->mainValue(), 'f', yDecimal);
                        if (m_isRecipe)
                        {
                            plotData.source = QString("ID:%1").arg(itor->sortKey());
                        }
                        else
                        {
                            plotData.source = QDateTime::fromSecsSinceEpoch(itor->sortKey()).toString(m_fxFormatStr);
                        }
                        
                        displayDataList.append(plotData);
                    }
                    else if (itor != curveDatas->constBegin() && (itor->mainKey() < xValueTemp && std::prev(itor)->mainKey() > xValueTemp
                        || itor->mainKey() > xValueTemp && std::prev(itor)->mainKey() < xValueTemp))
                    {
                        auto ratio = (itor->mainValue() - std::prev(itor)->mainValue()) / (itor->mainKey() - std::prev(itor)->mainKey());
                        auto dataValue = (xValue - std::prev(itor)->mainKey()) * ratio + std::prev(itor)->mainValue();
                        plotData.yValue = QString::number(dataValue, 'f', yDecimal);

                        ratio = (itor->sortKey() - std::prev(itor)->sortKey()) / (itor->mainKey() - std::prev(itor)->mainKey());

                        dataValue = (xValue - std::prev(itor)->mainKey()) * ratio + std::prev(itor)->sortKey();
                        if (m_isRecipe)
                        {
                            plotData.source = QString("ID:%1").arg(itor->sortKey());
                        }
                        else
                        {
                            plotData.source = QDateTime::fromSecsSinceEpoch(dataValue).toString(m_fxFormatStr);
                        }
                        
                        displayDataList.append(plotData);
                    }
                    
                }

            }
        }
        ruleData.displayDataList = displayDataList;

        auto fxRulerItemTextPtr = dynamic_cast<FxTrendRulerItemText*>(m_pRulerItemText);
        if (fxRulerItemTextPtr != nullptr)
        {
            fxRulerItemTextPtr->SetRulerData(ruleData);
            fxRulerItemTextPtr->position->setCoords(xValue, m_pPlot->yAxis->range().upper);
            fxRulerItemTextPtr->layer()->replot();
        }
    }
    else //暂时无用
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
