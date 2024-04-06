#include "TrendRulerItemText.h"
#include "FxTrendRulerItemText.h"
using namespace Graph;

FxTrendRulerItemText::FxTrendRulerItemText(QCustomPlot* parentPlot)
    : TrendRulerItemText(parentPlot)
{
    m_plotCurves.clear();
}

void FxTrendRulerItemText::draw(QCPPainter* painter)
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

        //auto timeTextRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, QString("X:" + m_FxRulerData.xValue));
        QString title(" X：" + m_FxRulerData.xValue);
        auto timeTextRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, title);
        auto timeRect = QRectF(textBoxRect.topLeft() + QPointF(10, 5), timeTextRect.size());
        painter->setPen(Qt::black);
        //painter->drawText(timeRect, "X:"+ m_FxRulerData.xValue);
        painter->drawText(timeRect, title);
        double height = timeRect.y() + timeRect.height() + 5;
        for (const auto& plotData : m_FxRulerData.displayDataList)
        {
            if (!plotData.isVisible)
            {
                continue;
            }
            painter->setBrush(GlobalDefine::GetQColorFromQRGB(plotData.lineColor));
            auto plotTextRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, 
                        GlobalDefine::ElideText(5, plotData.trendName) + "：" + plotData.yValue + "\n" + plotData.source);
            auto plotRect = QRectF(textBoxRect.topLeft() + QPointF(20, height), plotTextRect.size());
            painter->drawEllipse(QPointF(textBoxRect.topLeft().x() + 10, plotRect.center().y() - 8), 5, 5);
            painter->drawText(plotRect, Qt::AlignLeft, GlobalDefine::ElideText(5, plotData.trendName) + "："
                +plotData.yValue + "\n" + plotData.source);
            height = plotRect.y() + plotRect.height() + 5;
            if (height > m_height)
            {
                break;
            }
        }
    }
}

void Graph::FxTrendRulerItemText::wheelEvent(QWheelEvent* event)
{

}

void FxTrendRulerItemText::SetRulerData(const FxRulerData& rulerData)
{ 
     m_FxRulerData = rulerData; 

     QFontMetrics fontMetrics(m_font);
     /*QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, m_FxRulerData.time);*/
     QString title(" X："+ m_FxRulerData.xValue);
     QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, title);
     double rectWidth = 0;
     auto timeRect = QRectF(QPointF(0, 0) + QPointF(10, 5), textRect.size());
     double tempHeight = timeRect.y() + timeRect.height() + 5;
     rectWidth = qMax(rectWidth, timeRect.width() + 30);
     for (const auto& plotData : m_FxRulerData.displayDataList)
     {
         if (!plotData.isVisible)
         {
             continue;
         }
         auto plotTextRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | mTextAlignment,
             GlobalDefine::ElideText(5, plotData.trendName) + "：" + plotData.yValue + "\n" + plotData.source);
         auto plotRect = QRectF(QPointF(0, 0) + QPointF(30, tempHeight), plotTextRect.size());
         tempHeight = plotRect.y() + plotRect.height() + 5;
         rectWidth = qMax(rectWidth, plotRect.width() + 30);
         if (tempHeight> m_height)
         {
             break;
         }
     }
     tempHeight += 5;
     m_rect = QRectF(QPointF(0, 0), QSizeF(rectWidth, tempHeight));
}

void Graph::FxTrendRulerItemText::SetCurvesParam(const QList<QCPCurve*>& curves)
{
    m_plotCurves = curves;
}

const QList<QCPCurve*>& Graph::FxTrendRulerItemText::GetCurvesParam()
{
    return m_plotCurves;
}

void Graph::FxTrendRulerItemText::SetRulerTextHeight(int hei)
{
    m_height = hei;
}

