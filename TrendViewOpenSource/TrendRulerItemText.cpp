#include "TrendRulerItemText.h"
using namespace Graph;

TrendRulerItemText::TrendRulerItemText(QCustomPlot* parentPlot)
    : QCPItemText(parentPlot)
    , m_pPlot(parentPlot)
{
    mTextAlignment = Qt::AlignVCenter | Qt::AlignHCenter;
}

void TrendRulerItemText::draw(QCPPainter* painter)
{
    QPointF pos(position->pixelPosition());
    QTransform transform = painter->transform();
    transform.translate(pos.x(), pos.y());
    if (!qFuzzyIsNull(mRotation))
    {
        transform.rotate(mRotation);
    }

    painter->setFont(m_font);
    QRect textRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, mText);
    QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
    QPointF textPos = getTextDrawPoint(QPointF(0, 0), textBoxRect, mPositionAlignment); // 0, 0 because the transform does the translation
    textRect.moveTopLeft(textPos.toPoint() + QPoint(mPadding.left(), mPadding.top()));
    textBoxRect.moveTopLeft(textPos.toPoint());
    int clipPad = qCeil(mainPen().widthF());
    QRect boundingRect = textBoxRect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
    
    auto tempRect = painter->transform().mapRect(m_pPlot->axisRect()->rect());
    if (tempRect.width() > 0 && tempRect.height() > 0 && transform.mapRect(boundingRect).intersects(tempRect))
    {
        painter->setTransform(transform);
        textBoxRect.setX(textBoxRect.x() + 10);
        m_rect.setTopLeft(textBoxRect.topLeft());
        auto tempRect = m_rect;
        UpdatePosition(tempRect);
        textBoxRect.setX(tempRect.x());

        QLinearGradient linear(0, 0, 0, tempRect.height());
        linear.setColorAt(0, QColor("#FFFFFF"));
        linear.setColorAt(1, QColor("#EFEFEF"));
        linear.setSpread(QGradient::PadSpread);
        painter->setBrush(linear);

        painter->setPen(QColor("#CFD5D8"));
        painter->drawRoundedRect(tempRect, 4, 4);

        auto timeTextRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, m_rulerData.time);
        auto timeRect = QRectF(textBoxRect.topLeft() + QPointF(10, 10), timeTextRect.size());
        painter->setPen(Qt::black);
        painter->drawText(timeRect, m_rulerData.time);
        double height = timeRect.y() + timeRect.height() + 5;
        for (const auto& plotData : m_rulerData.displayDataList)
        {
            if (!plotData.isVisible)
            {
                continue;
            }
            painter->setBrush(GlobalDefine::GetQColorFromQRGB(plotData.lineColor));
            auto plotTextRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, 
                        GlobalDefine::ElideText(6, plotData.trendName) + "：" + plotData.data);
            auto plotRect = QRectF(textBoxRect.topLeft() + QPointF(30, height), plotTextRect.size());
            painter->drawEllipse(QPointF(textBoxRect.topLeft().x() + 15, plotRect.center().y() + 2), 5, 5);
            painter->drawText(plotRect, mTextAlignment, GlobalDefine::ElideText(6, plotData.trendName) + "：" + plotData.data);
            height = plotRect.y() + plotRect.height() + 5;
        }
    }
}

double TrendRulerItemText::selectTest(const QPointF& pos, bool onlySelectable, QVariant* details) const
{
    Q_UNUSED(details)
    if (onlySelectable && !mSelectable)
    {
        return -1;
    }
    QRectF textBoxRect(position->pixelPosition(), QSizeF(m_rect.width(), m_rect.height()));
    return rectDistance(textBoxRect, pos, true);
}

void TrendRulerItemText::SetRulerData(const RulerData& rulerData)
{ 
    m_rulerData = rulerData; 

     QFontMetrics fontMetrics(m_font);
     QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, m_rulerData.time);
     double rectWidth = 0;
     auto timeRect = QRectF(QPointF(0, 0) + QPointF(10, 5), textRect.size());
     double tempHeight = timeRect.y() + timeRect.height() + 5;
     rectWidth = qMax(rectWidth, timeRect.width() + 20);
     for (const auto& plotData : m_rulerData.displayDataList)
     {
         if (!plotData.isVisible)
         {
             continue;
         }
         auto plotTextRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment,
                                GlobalDefine::ElideText(6, plotData.trendName) + "：" + plotData.data);
         auto plotRect = QRectF(QPointF(0, 0) + QPointF(30, tempHeight), plotTextRect.size());
         tempHeight = plotRect.y() + plotRect.height() + 5;
         rectWidth = qMax(rectWidth, plotRect.width() + 40);
     }
     tempHeight += 5;
     m_rect = QRectF(QPointF(0, 0), QSizeF(rectWidth, tempHeight));
}

void TrendRulerItemText::UpdatePosition(QRectF& rect)
{
    if (!GetIsRunning())
    {
        return;
    }

    auto plotRect = m_pPlot->viewport(); 
    auto rectTopRight = rect.x() + m_pPlot->xAxis->coordToPixel(position->coords().x()) + rect.width();
    if (rectTopRight > plotRect.width())
    {
        auto rectWidth = rect.width();
        rect.setX(rect.topLeft().x() - rect.width() - 10);
        rect.setWidth(rectWidth);
    }
    else
    {
        rect = m_rect;
    }
}