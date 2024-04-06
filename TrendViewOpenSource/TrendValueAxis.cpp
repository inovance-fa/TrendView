#include "TrendValueAxis.h"
#include "TrendValueAxisSticker.h"
#include <QSharedPointer>
using namespace Graph;

TrendValueAxis::TrendValueAxis(QCPAxisRect *parent, AxisType type)
    : QCPAxis(parent, type)
{
    if (nullptr != mAxisPainter)
    {
        delete mAxisPainter;
        mAxisPainter = new TrendValueAxisPainter(parent->parentPlot(), this);
    }
}

double TrendValueAxis::pixelToCoord(double value) const
{
    if (orientation() == Qt::Horizontal)
    {
        if (mAxisRect->width() <= 0)
        {
            return 0.0;
        }
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
            {
                return (value - mAxisRect->left()) / double(mAxisRect->width()) * mRange.size() + mRange.lower;
            }
            else
            {
                return -(value - mAxisRect->left()) / double(mAxisRect->width()) * mRange.size() + mRange.upper;
            }
        }
        else // mScaleType == stLogarithmic
        {
            if (!mRangeReversed)
            {
                return qPow(mRange.upper / mRange.lower, (value - mAxisRect->left()) / double(mAxisRect->width())) * mRange.lower;
            }
            else
            {
                return qPow(mRange.upper / mRange.lower, (mAxisRect->left() - value) / double(mAxisRect->width())) * mRange.upper;
            }
        }
    }
    else // orientation() == Qt::Vertical
    {
        if (mAxisRect->height() <= 0)
        {
            return 0.0;
        }
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
            {
                return (mAxisRect->bottom() - value) / double(mAxisRect->height()) * mRange.size() + mRange.lower;
            }
            else
            {
                return -(mAxisRect->bottom() - value) / double(mAxisRect->height()) * mRange.size() + mRange.upper;
            }
        }
        else // mScaleType == stLogarithmic
        {
            if (!mRangeReversed)
            {
                return qPow(mRange.upper / mRange.lower, (mAxisRect->bottom() - value) / double(mAxisRect->height())) * mRange.lower;
            }
            else
            {
                return qPow(mRange.upper / mRange.lower, (value - mAxisRect->bottom()) / double(mAxisRect->height())) * mRange.upper;
            }
        }
    }
}

TrendValueAxisPainter::TrendValueAxisPainter(QCustomPlot* parentPlot, QCPAxis* axis)
    : QCPAxisPainterPrivate(parentPlot)
    , m_pPlot(parentPlot)
    , m_pAxis(axis)
{
}

void TrendValueAxisPainter::draw(QCPPainter* painter)
{
    if (m_pPlot && m_pPlot->axisRect())
    {
        if (QCPAxis::atLeft == type || QCPAxis::atRight == type)
        {
            if (m_pPlot->axisRect()->rect().height() <= 0)
            {
                return;
            }
        }
        else
        {
            if (m_pPlot->axisRect()->rect().width() <= 0)
            {
                return;
            }
        }
    }

    QByteArray newHash = generateLabelParameterHash();
    if (newHash != mLabelParameterHash)
    {
        mLabelCache.clear();
        mLabelParameterHash = newHash;
    }

    QPoint origin;
    switch (type)
    {
    case QCPAxis::atLeft:
    {
        origin = axisRect.bottomLeft() + QPoint(-offset, 0);
        break;
    }
    case QCPAxis::atRight:
    {
        origin = axisRect.bottomRight() + QPoint(+offset, 0);
        break;
    }
    case QCPAxis::atTop:
    {
        origin = axisRect.topLeft() + QPoint(0, -offset);
        break;
    }
    case QCPAxis::atBottom:
    {
        origin = axisRect.bottomLeft() + QPoint(0, +offset);
        break;
    }
    }

    double xCor = 0, yCor = 0;
    switch (type)
    {
    case QCPAxis::atTop:
    {
        yCor = -1;
        break;
    }
    case QCPAxis::atRight:
    {
        xCor = 1;
        break;
    }
    default:
        break;
    }
    int margin = 0;
    //绘制基线
    QLineF baseLine = DrawBaseline(painter, origin, xCor, yCor);
    //绘制主刻度
    DrawTicks(painter, origin, xCor, yCor);
    //绘制次刻度
    DrawSubticks(painter, origin, xCor, yCor);  
    margin += qMax(0, qMax(tickLengthOut, subTickLengthOut));
    //绘制轴结束标记
    DrawAxisBaseEndings(painter, baseLine);
    //绘制刻度标签
    QSize tickLabelsSize = DrawTickLabels(painter, margin);
    //绘制轴标签
    QRect labelBounds = DrawAxisLabel(painter, origin, margin);
    SetSelectionBoxes(painter, origin, tickLabelsSize, labelBounds);
    
}

QLineF TrendValueAxisPainter::DrawBaseline(QCPPainter* painter, const QPoint& origin, double xCor, double yCor)
{
    QLineF baseLine;
    painter->setPen(basePen);
    if (QCPAxis::orientation(type) == Qt::Horizontal)
    {
        baseLine.setPoints(origin + QPointF(xCor, yCor), origin + QPointF(axisRect.width() + xCor, yCor));
    }
    else
    {
        baseLine.setPoints(origin + QPointF(xCor, yCor), origin + QPointF(xCor, -axisRect.height() + yCor));
    }
    if (reversedEndings)
    {
        baseLine = QLineF(baseLine.p2(), baseLine.p1());
    }
    painter->drawLine(baseLine);
    return baseLine;
}

void TrendValueAxisPainter::DrawTicks(QCPPainter* painter, const QPoint& origin, double xCor, double yCor)
{
    if (!tickPositions.isEmpty())
    {
        painter->setPen(tickPen);
        int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1;
        if (QCPAxis::orientation(type) == Qt::Horizontal)
        {
            foreach(double tickPos, tickPositions)
            {
                painter->drawLine(QLineF(tickPos + xCor, origin.y() - tickLengthOut * tickDir + yCor, tickPos + xCor, origin.y() + tickLengthIn * tickDir + yCor));
            }
        }
        else
        {
            bool isNeedPaintUpper = true;
            auto trendTicker = dynamic_cast<TrendValueAxisSticker*>(m_pAxis->ticker().data());
            if (nullptr != trendTicker)
            {
                isNeedPaintUpper = trendTicker->GetIsNeedPaintUpperLabel();
            }
            for (int i = 0; i < tickPositions.size(); ++i)
            {
                if (i == tickPositions.size() - 1)
                {
                    if (isNeedPaintUpper)
                    {
                        painter->drawLine(QLineF(origin.x() - tickLengthOut * tickDir + xCor, tickPositions[i] + yCor, origin.x() + tickLengthIn * tickDir + xCor, tickPositions[i] + yCor));
                    }
                }
                else
                {
                    painter->drawLine(QLineF(origin.x() - tickLengthOut * tickDir + xCor, tickPositions[i] + yCor, origin.x() + tickLengthIn * tickDir + xCor, tickPositions[i] + yCor));
                }
            }
        }
    }
}

void TrendValueAxisPainter::DrawSubticks(QCPPainter* painter, const QPoint& origin, double xCor, double yCor)
{
    if (!subTickPositions.isEmpty())
    {
        painter->setPen(subTickPen);
        int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1;
        if (QCPAxis::orientation(type) == Qt::Horizontal)
        {
            foreach(double subTickPos, subTickPositions)
            {
                painter->drawLine(QLineF(subTickPos + xCor, origin.y() - subTickLengthOut * tickDir + yCor, subTickPos + xCor, origin.y() + subTickLengthIn * tickDir + yCor));
            }
        }
        else
        {
            foreach(double subTickPos, subTickPositions)
            {
                painter->drawLine(QLineF(origin.x() - subTickLengthOut * tickDir + xCor, subTickPos + yCor, origin.x() + subTickLengthIn * tickDir + xCor, subTickPos + yCor));
            }
        }
    }
}

void TrendValueAxisPainter::DrawAxisBaseEndings(QCPPainter* painter, const QLineF& baseLine)
{
    bool antialiasingBackup = painter->antialiasing();
    painter->setAntialiasing(true);
    painter->setBrush(QBrush(basePen.color()));
    QCPVector2D baseLineVector(baseLine.dx(), baseLine.dy());
    if (lowerEnding.style() != QCPLineEnding::esNone)
    {
        lowerEnding.draw(painter, QCPVector2D(baseLine.p1()) - baseLineVector.normalized() * lowerEnding.realLength() * (lowerEnding.inverted() ? -1 : 1), -baseLineVector);
    }
    if (upperEnding.style() != QCPLineEnding::esNone)
    {
        upperEnding.draw(painter, QCPVector2D(baseLine.p2()) + baseLineVector.normalized() * upperEnding.realLength() * (upperEnding.inverted() ? -1 : 1), baseLineVector);
    }
    painter->setAntialiasing(antialiasingBackup);
}

QSize TrendValueAxisPainter::DrawTickLabels(QCPPainter* painter, int& margin)
{
    QRect oldClipRect;
    if (tickLabelSide == QCPAxis::lsInside)
    {
        oldClipRect = painter->clipRegion().boundingRect();
        painter->setClipRect(axisRect);
    }
    QSize tickLabelsSize(0, 0);
    if (!tickLabels.isEmpty())
    {
        if (tickLabelSide == QCPAxis::lsOutside)
        {
            margin += tickLabelPadding;
        }
        painter->setFont(tickLabelFont);
        painter->setPen(QPen(tickLabelColor));
        const int maxLabelIndex = qMin(tickPositions.size(), tickLabels.size());
        int distanceToAxis = margin;
        if (tickLabelSide == QCPAxis::lsInside)
        {
            distanceToAxis = -(qMax(tickLengthIn, subTickLengthIn) + tickLabelPadding);
        }

        bool isNeedPaintUpper = true;
        auto trendTicker = dynamic_cast<TrendValueAxisSticker*>(m_pAxis->ticker().data());
        if (nullptr != trendTicker)
        {
            isNeedPaintUpper = trendTicker->GetIsNeedPaintUpperLabel();
        }
        for (int i = 0; i < maxLabelIndex; ++i)
        {
            if (i == maxLabelIndex - 1)
            {
                if (isNeedPaintUpper)
                {
                    placeTickLabel(painter, tickPositions.at(i), distanceToAxis, tickLabels.at(i), &tickLabelsSize);
                }              
            }
            else
            {
                placeTickLabel(painter, tickPositions.at(i), distanceToAxis, tickLabels.at(i), &tickLabelsSize);
            }          
        }
        if (tickLabelSide == QCPAxis::lsOutside)
        {
            margin += (QCPAxis::orientation(type) == Qt::Horizontal) ? tickLabelsSize.height() : tickLabelsSize.width();
        }
    }
    if (tickLabelSide == QCPAxis::lsInside)
    {
        painter->setClipRect(oldClipRect);
    }
    return tickLabelsSize;
}

QRect TrendValueAxisPainter::DrawAxisLabel(QCPPainter* painter,const QPoint& origin, int& margin)
{
     QRect labelBounds;
     if (!label.isEmpty())
     {
         margin += labelPadding;
         painter->setFont(labelFont);
         painter->setPen(QPen(labelColor));
         labelBounds = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip, label);
         if (type == QCPAxis::atLeft)
         {
             QTransform oldTransform = painter->transform();
             painter->translate((origin.x() - margin - labelBounds.height()), origin.y());
             painter->rotate(-90);
 
             QPixmap labelPixmap(axisRect.height(), labelBounds.height());
             labelPixmap.fill(Qt::transparent);
             QPainter labelPixmapPainter(&labelPixmap);
             labelPixmapPainter.setFont(labelFont);
             labelPixmapPainter.setPen(QPen(labelColor));
             labelPixmapPainter.drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
             painter->drawPixmap(0, 0, labelPixmap);
             painter->setTransform(oldTransform);
         }
         else if (type == QCPAxis::atRight)
         {
             QTransform oldTransform = painter->transform();
             painter->translate((origin.x() + margin + labelBounds.height()), origin.y() - axisRect.height());
             painter->rotate(90);
 
             QPixmap labelPixmap(axisRect.height(), labelBounds.height());
             labelPixmap.fill(Qt::transparent);
             QPainter labelPixmapPainter(&labelPixmap);
             labelPixmapPainter.setFont(labelFont);
             labelPixmapPainter.setPen(QPen(labelColor));
             labelPixmapPainter.drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
             painter->drawPixmap(0, 0, labelPixmap);
             painter->setTransform(oldTransform);
         }
         else if (type == QCPAxis::atTop)
         {
             QPixmap labelPixmap(axisRect.width(), labelBounds.height());
             labelPixmap.fill(Qt::transparent);
             QPainter labelPixmapPainter(&labelPixmap);
             labelPixmapPainter.setFont(labelFont);
             labelPixmapPainter.setPen(QPen(labelColor));
             labelPixmapPainter.drawText(0, 0, axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
             painter->drawPixmap(0, 0, labelPixmap);
         }
         else if (type == QCPAxis::atBottom)
         {
             QPixmap labelPixmap(axisRect.width(), labelBounds.height());
             labelPixmap.fill(Qt::transparent);
             QPainter labelPixmapPainter(&labelPixmap);
             labelPixmapPainter.setFont(labelFont);
             labelPixmapPainter.setPen(QPen(labelColor));
             labelPixmapPainter.drawText(0, 0, axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
             painter->drawPixmap(origin.x(), origin.y() + margin , labelPixmap);
         }
     }
     return labelBounds;
}

void TrendValueAxisPainter::SetSelectionBoxes(QCPPainter* painter, const QPoint& origin, const QSize& tickLabelsSize, const QRect& labelBounds)
{
    int selectionTolerance = 0;
    if (mParentPlot)
    {
        selectionTolerance = mParentPlot->selectionTolerance();
    }
    int selAxisOutSize = qMax(qMax(tickLengthOut, subTickLengthOut), selectionTolerance);
    int selAxisInSize = selectionTolerance;
    int selTickLabelSize;
    int selTickLabelOffset;
    if (tickLabelSide == QCPAxis::lsOutside)
    {
        selTickLabelSize = (QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
        selTickLabelOffset = qMax(tickLengthOut, subTickLengthOut) + tickLabelPadding;
    }
    else
    {
        selTickLabelSize = -(QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
        selTickLabelOffset = -(qMax(tickLengthIn, subTickLengthIn) + tickLabelPadding);
    }
    int selLabelSize = labelBounds.height();
    int selLabelOffset = qMax(tickLengthOut, subTickLengthOut) + (!tickLabels.isEmpty() && tickLabelSide == QCPAxis::lsOutside ? tickLabelPadding + selTickLabelSize : 0) + labelPadding;
    if (type == QCPAxis::atLeft)
    {
        mAxisSelectionBox.setCoords(origin.x() - selAxisOutSize, axisRect.top(), origin.x() + selAxisInSize, axisRect.bottom());
        mTickLabelsSelectionBox.setCoords(origin.x() - selTickLabelOffset - selTickLabelSize, axisRect.top(), origin.x() - selTickLabelOffset, axisRect.bottom());
        mLabelSelectionBox.setCoords(origin.x() - selLabelOffset - selLabelSize, axisRect.top(), origin.x() - selLabelOffset, axisRect.bottom());
    }
    else if (type == QCPAxis::atRight)
    {
        mAxisSelectionBox.setCoords(origin.x() - selAxisInSize, axisRect.top(), origin.x() + selAxisOutSize, axisRect.bottom());
        mTickLabelsSelectionBox.setCoords(origin.x() + selTickLabelOffset + selTickLabelSize, axisRect.top(), origin.x() + selTickLabelOffset, axisRect.bottom());
        mLabelSelectionBox.setCoords(origin.x() + selLabelOffset + selLabelSize, axisRect.top(), origin.x() + selLabelOffset, axisRect.bottom());
    }
    else if (type == QCPAxis::atTop)
    {
        mAxisSelectionBox.setCoords(axisRect.left(), origin.y() - selAxisOutSize, axisRect.right(), origin.y() + selAxisInSize);
        mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y() - selTickLabelOffset - selTickLabelSize, axisRect.right(), origin.y() - selTickLabelOffset);
        mLabelSelectionBox.setCoords(axisRect.left(), origin.y() - selLabelOffset - selLabelSize, axisRect.right(), origin.y() - selLabelOffset);
    }
    else if (type == QCPAxis::atBottom)
    {
        mAxisSelectionBox.setCoords(axisRect.left(), origin.y() - selAxisInSize, axisRect.right(), origin.y() + selAxisOutSize);
        mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y() + selTickLabelOffset + selTickLabelSize, axisRect.right(), origin.y() + selTickLabelOffset);
        mLabelSelectionBox.setCoords(axisRect.left(), origin.y() + selLabelOffset + selLabelSize, axisRect.right(), origin.y() + selLabelOffset);
    }
    mAxisSelectionBox = mAxisSelectionBox.normalized();
    mTickLabelsSelectionBox = mTickLabelsSelectionBox.normalized();
    mLabelSelectionBox = mLabelSelectionBox.normalized();
}