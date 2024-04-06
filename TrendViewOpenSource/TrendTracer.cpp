#include "qcustomplot.h"
#include "TrendTracer.h"
#include "TrendRulerItemText.h"
using namespace Graph;

TrendTracer::TrendTracer(QCustomPlot* plot, TracerType type, QObject* parent)
    : QObject(parent)
    , m_type(type)
    , m_plot(plot)
{
    Initial();
}

TrendTracer::~TrendTracer()
{
    if (nullptr == m_plot)
    {
        return;
    }

    if (m_tracer)
    {
        m_plot->removeItem(m_tracer);
    }
    if (m_label)
    {
        m_plot->removeItem(m_label);
    }
    if (m_arrow)
    {
        m_plot->removeItem(m_arrow);
    }
}

void TrendTracer::Initial()
{
    if (nullptr == m_plot)
    {
        return;
    }

    QColor clrDefault(0, 0, 0);
    QBrush brushDefault(Qt::NoBrush);
    QPen penDefault(clrDefault);
    penDefault.setWidthF(0.5);

    m_tracer = new QCPItemTracer(m_plot);
    m_tracer->setClipToAxisRect(false);
    m_tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
    m_tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);
    m_tracer->setStyle(QCPItemTracer::/*tsCircle*/tsNone);
    m_tracer->setPen(penDefault);
    m_tracer->setBrush(brushDefault);
    m_tracer->position->setCoords(m_plot->xAxis->range().lower , m_plot->yAxis->range().upper);

    m_label = m_type == XAxisTracer ? new TrendRulerItemText(m_plot) : new QCPItemText(m_plot);
    m_label->setLayer("overlay");
    m_label->setClipToAxisRect(false);
    m_label->setPadding(QMargins(5, 5, 5, 5));
    m_label->setBrush(brushDefault);
    m_label->setPen(penDefault);
    m_label->setFont(QFont("宋体", 12));
    m_label->setColor(clrDefault);
    m_label->setText("");

    switch (m_type)
    {
    case TrendTracer::DataTracer:
    {
        m_arrow = new QCPItemLine(m_plot);
        QPen  arrowPen(clrDefault, 1);
        m_arrow->setPen(penDefault);
        m_arrow->setLayer("overlay");
        m_arrow->setClipToAxisRect(false);
        m_arrow->setHead(QCPLineEnding::esSpikeArrow);//设置头部为箭头形状

        m_tracer->setSize(5);
        m_label->position->setParentAnchor(m_tracer->position);
        m_label->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_label->position->setCoords(20, 0);

        m_arrow->end->setParentAnchor(m_tracer->position);
        m_arrow->start->setParentAnchor(m_arrow->end);
        m_arrow->start->setCoords(20, 0);   
        break;
    }
    case TrendTracer::XAxisTracer:
    {
        m_tracer->setSize(7);
        m_label->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);
        m_label->setSelectable(true);
        m_label->position->setParentAnchor(m_tracer->position);
        m_label->position->setCoords(0, 0);     
        break;
    }
    default:
        break;
    }

    SetVisible(false);
}

void TrendTracer::SetColor(const QRgb& color)
{
    QPen pen(GlobalDefine::GetQColorFromQRGB(color));

    if (m_tracer)
    {
        m_tracer->setPen(pen);
    }     
    if (m_arrow)
    {
        m_arrow->setPen(pen);
    }
    if (m_type == TrendTracer::XAxisTracer)
    {
        m_label->setBrush(pen.color());
    }
}

void TrendTracer::SetBrush(const QBrush& brush)
{
    if (m_tracer)
    {
        m_tracer->setBrush(brush);
    }
}

void TrendTracer::SetText(const QString& text)
{
    if (m_label)
    {
        m_label->setText(text);
    }
}

void TrendTracer::SetLabelPen(const QPen& pen)
{
    if (m_label)
    {
        m_label->setPen(pen);
        m_label->setBrush(Qt::NoBrush);
        m_label->setColor(pen.color());
    }
}

void TrendTracer::UpdatePosition(qreal xValue, qreal yValue, bool isInValid)
{
    if (yValue > m_plot->yAxis->range().upper)
    {
        yValue = m_plot->yAxis->range().upper;
    }

    switch (m_type)
    {
    case TrendTracer::DataTracer:
    {
        m_tracer->position->setCoords(xValue, yValue);
        QString text = isInValid ? "???" : QString("%1").arg(yValue);
        SetText(text);
        break;
    }
    case TrendTracer::XAxisTracer:
    {
        m_tracer->position->setCoords(xValue, m_plot->yAxis->range().upper);
        m_label->position->setCoords(0, 0);
        SetText(QDateTime::fromSecsSinceEpoch(xValue)
                .toString(((QCPAxisTickerDateTime*)(m_plot->xAxis->ticker().data()))->dateTimeFormat()));
        break;
    }
    default:
        break;
    }
}

void TrendTracer::SetVisible(bool isVisible)
{
    m_isVisible = isVisible;

    if (m_tracer)
    {
        m_tracer->setVisible(m_isVisible);
    }
    if (m_label)
    {
        m_label->setVisible(m_isVisible);
    }
    if (m_arrow)
    {
        m_arrow->setVisible(m_isVisible);
    }
}

void TrendTracer::SetSelected(bool isSelect)
{
    if (m_tracer)
    {
        m_tracer->setSelected(isSelect);
    }
    if (m_label)
    {
        m_label->setSelected(isSelect);
    }
    if (m_arrow)
    {
        m_arrow->setSelected(isSelect);
    }
}

void TrendTracer::SetTracerFont(const QFont& font)
{
    if (m_label)
    {
        m_label->setFont(font);
    }
}