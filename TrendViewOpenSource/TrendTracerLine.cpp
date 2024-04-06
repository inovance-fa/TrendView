#include "TrendTracerLine.h"
using namespace Graph;

TrendTracerLine::TrendTracerLine(QCustomPlot* plot)
    : QCPItemLine(plot)
    , m_plot(plot)
{
    InitLine();
}

void TrendTracerLine::InitLine()
{
    if (nullptr == m_plot)
    {
        return;
    }

    QPen linesPen(QBrush(QColor("#717171")), 2, Qt::DashLine);
    //垂直线
    setLayer("overlay");
    setPen(linesPen);
    setClipToAxisRect(false);
    setSelectable(true);

    start->setTypeX(QCPItemPosition::ptPlotCoords);
    start->setTypeY(QCPItemPosition::ptPlotCoords);
    end->setTypeX(QCPItemPosition::ptPlotCoords);
    end->setTypeY(QCPItemPosition::ptPlotCoords);
    start->setCoords(m_plot->xAxis->range().lower, m_plot->yAxis->range().lower);
    end->setCoords(m_plot->xAxis->range().lower, m_plot->yAxis->range().upper);
}

void TrendTracerLine::UpdatePosition(qreal xValue)
{
    if (nullptr == m_plot)
    {
        return;
    }
    start->setCoords(xValue, m_plot->yAxis->range().lower);
    if (m_plot->axisRect() && m_plot->axisRect()->rect().height() <= 0)
    {
        end->setCoords(xValue, m_plot->yAxis->range().lower);
    }
    else
    {
        end->setCoords(xValue, m_plot->yAxis->range().upper);
    }
}   

void TrendTracerLine::SetColor(const QRgb& color)
{
    QPen linesPen(GlobalDefine::GetQColorFromQRGB(color), 2, Qt::DashLine);
    setPen(linesPen);
}