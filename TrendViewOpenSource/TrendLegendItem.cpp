#include "TrendLegendItem.h"
using namespace Graph;

TrendLegendItem::TrendLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable)
    : QCPPlottableLegendItem(parent, plottable)
{
    mParentLegend->setIconTextPadding(2);
}

void TrendLegendItem::draw(QCPPainter* painter)
{
    if (nullptr == mPlottable)
    {
        return;
    }

    painter->setFont(getFont());
    painter->setPen(QPen(getTextColor()));
    QSize iconSize = mParentLegend->iconSize();
    QRect textRect = painter->fontMetrics().boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, 
        GlobalDefine::ElideText(6, mPlottable->name()));
    QRect iconRect(mRect.topLeft(), QSize(iconSize.width(), textRect.height()));
    int textHeight = qMax(textRect.height(), iconSize.height());
    painter->drawText(mRect.x() + iconSize.width() + mParentLegend->iconTextPadding(), mRect.y(), 
        textRect.width(), textHeight, Qt::TextDontClip, GlobalDefine::ElideText(6, mPlottable->name()));

    painter->save();
    painter->setClipRect(iconRect, Qt::IntersectClip);
    DrawLegendIcon(painter, iconRect);
    painter->restore();

    if (getIconBorderPen().style() != Qt::NoPen)
    {
        painter->setPen(getIconBorderPen());
        painter->setBrush(Qt::NoBrush);
        int halfPen = qCeil(painter->pen().widthF() * 0.5) + 1;
        painter->setClipRect(mOuterRect.adjusted(-halfPen, -halfPen, halfPen, halfPen));
        painter->drawRect(iconRect);
    }
}

QSize TrendLegendItem::minimumOuterSizeHint() const
{
    if (nullptr == mPlottable)
    {
        return {};
    }
    QSize result(0, 0);
    QRect textRect;
    QFontMetrics fontMetrics(getFont());
    QSize iconSize = mParentLegend->iconSize();
    textRect = fontMetrics.boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, GlobalDefine::ElideText(6, mPlottable->name()));
    result.setWidth(iconSize.width() + mParentLegend->iconTextPadding() + textRect.width());
    result.setHeight(qMax(textRect.height(), iconSize.height()));
    result.rwidth() += mMargins.left() + mMargins.right();
    result.rheight() += mMargins.top() + mMargins.bottom();

    return result;
}

void TrendLegendItem::DrawLegendIcon(QCPPainter* painter, const QRectF& rect) const
{
    if (nullptr == mPlottable)
    {
        return;
    }
    if (mPlottable->brush().style() != Qt::NoBrush)
    {
        ApplyAntialiasingHint(painter, mPlottable->antialiasedFill(), QCP::aeFills);
        painter->fillRect(QRectF(rect.left(), rect.top() + rect.height() / 2.0, rect.width(), rect.height() / 3.0), 
                                mPlottable->brush());
    }
    auto graph = dynamic_cast<QCPGraph*>(mPlottable);
    auto pen = mPlottable->pen();
    pen.setWidth(2.0);
    if (graph && graph->lineStyle() != QCPGraph::LineStyle::lsNone)
    {
        ApplyAntialiasingHint(painter, antialiased(), QCP::aeLegendItems);
        painter->setPen(pen);
        painter->drawLine(QLineF(rect.left(), rect.top() + rect.height() / 2.0, rect.right() + 5, 
                            rect.top() + rect.height() / 2.0));
    }
    if (graph && !graph->scatterStyle().isNone())
    {
        ApplyAntialiasingHint(painter, mPlottable->antialiasedScatters(), QCP::aeScatters);
        if (graph->scatterStyle().shape() == QCPScatterStyle::ssPixmap 
            && (graph->scatterStyle().pixmap().size().width() > rect.width() 
                || graph->scatterStyle().pixmap().size().height() > rect.height()))
        {
            QCPScatterStyle scaledStyle(graph->scatterStyle());
            scaledStyle.setPixmap(scaledStyle.pixmap().scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            scaledStyle.applyTo(painter, pen);
            scaledStyle.drawShape(painter, QRectF(rect).center());
        }
        else
        {
            QCPScatterStyle scaledStyle(graph->scatterStyle());
            scaledStyle.applyTo(painter, pen);
            scaledStyle.setSize(4.0);
            scaledStyle.drawShape(painter, QRectF(rect).center());
        }
    }
}

void TrendLegendItem::ApplyAntialiasingHint(QCPPainter* painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const
{
    if (nullptr == painter)
    {
        return;
    }

    if (mParentPlot && mParentPlot->notAntialiasedElements().testFlag(overrideElement))
    {
        painter->setAntialiasing(false);
    }
    else if (mParentPlot && mParentPlot->antialiasedElements().testFlag(overrideElement))
    {
        painter->setAntialiasing(true);
    }
    else
    {
        painter->setAntialiasing(localAntialiased);
    }
}