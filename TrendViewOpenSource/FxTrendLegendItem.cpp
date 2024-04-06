#include "FxTrendLegendItem.h"
using namespace Graph;

FxTrendLegendItem::FxTrendLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable)
    : TrendLegendItem(parent, plottable)
{
    
}

void FxTrendLegendItem::DrawLegendIcon(QCPPainter* painter, const QRectF& rect) const
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
    auto curve = dynamic_cast<QCPCurve*>(mPlottable);
    auto pen = mPlottable->pen();
    pen.setWidth(2.0);
    if (curve && curve->lineStyle() != QCPCurve::LineStyle::lsNone)
    {
        ApplyAntialiasingHint(painter, antialiased(), QCP::aeLegendItems);
        painter->setPen(pen);
        painter->drawLine(QLineF(rect.left(), rect.top() + rect.height() / 2.0, rect.right() + 5, 
                            rect.top() + rect.height() / 2.0));
    }
    if (curve && !curve->scatterStyle().isNone())
    {
        ApplyAntialiasingHint(painter, mPlottable->antialiasedScatters(), QCP::aeScatters);
        if (curve->scatterStyle().shape() == QCPScatterStyle::ssPixmap
            && (curve->scatterStyle().pixmap().size().width() > rect.width()
                || curve->scatterStyle().pixmap().size().height() > rect.height()))
        {
            QCPScatterStyle scaledStyle(curve->scatterStyle());
            scaledStyle.setPixmap(scaledStyle.pixmap().scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            scaledStyle.applyTo(painter, pen);
            scaledStyle.drawShape(painter, QRectF(rect).center());
        }
        else
        {
            QCPScatterStyle scaledStyle(curve->scatterStyle());
            scaledStyle.applyTo(painter, pen);
            scaledStyle.setSize(4.0);
            scaledStyle.drawShape(painter, QRectF(rect).center());
        }
    }
}

